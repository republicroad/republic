
## sqlalchemy

### foreign key


[SQLAlchemy - don't enforce foreign key constraint on a relationship](https://stackoverflow.com/a/7111348)  
[Data Management With Python, SQLite, and SQLAlchemy: relationships](https://realpython.com/python-sqlite-sqlalchemy/#building-relationships)
  
此博客对于 backref 和数据库外键的理解有误. 代码示例可以参考.
https://that.guru/blog/sqlalchemy-relationships-without-foreign-keys/




#### 使用 orm 建立实体关系但是数据库不建立真实外键

针对一些老的项目改造, 或者降低数据完整性的严格限制, 我们可以建立 orm model 间的各种填充和查找关系, 但是不在数据库上建立真实的物理外键.
这样方便我们使用 orm 的查询, 局部删除(比如一对多关系中一方删除)

- POINT-1: not having a ForeignKey neither on the RDBMS level nor on the SA level
- POINT-2: explicitly specify join conditions for the relationship
- POINT-3: mark relationship cascades to rely on passive_deletes flag

```python
from sqlalchemy import create_engine, Column, Integer, String, ForeignKey
from sqlalchemy.orm import scoped_session, sessionmaker, relationship
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()
engine = create_engine('sqlite:///:memory:', echo=False)

Session = sessionmaker(bind=engine)

class TestAuditLog(Base):
    __tablename__ = 'TEST_AUDIT_LOG'
    id = Column(Integer, primary_key=True)
    comment = Column(String)

    entityId = Column('TEST_AUDIT_LOG', Integer, nullable=False,
                     # POINT-1
                     #ForeignKey('TEST.TS_TEST_ID', ondelete="CASCADE"),
                     )

    def __init__(self, comment):
        self.comment = comment

    def __repr__(self):
        return "<TestAuditLog(id=%s entityId=%s, comment=%s)>" % (self.id, self.entityId, self.comment)

class Test(Base):
    __tablename__ = 'TEST'
    id = Column('TS_TEST_ID', Integer, primary_key=True)
    name = Column(String)

    audits = relationship(TestAuditLog, backref='test',
                # POINT-2
                primaryjoin="Test.id==TestAuditLog.entityId",
                foreign_keys=[TestAuditLog.__table__.c.TEST_AUDIT_LOG],
                # POINT-3
                passive_deletes='all',
            )

    def __init__(self, name):
        self.name = name

    def __repr__(self):
        return "<Test(id=%s, name=%s)>" % (self.id, self.name)


Base.metadata.create_all(engine)

###################
## tests
session = Session()

# create test data
tests = [Test("test-" + str(i)) for i in range(3)]
_cnt = 0
for _t in tests:
    for __ in range(2):
        _t.audits.append(TestAuditLog("comment-" + str(_cnt)))
        _cnt += 1
session.add_all(tests)
session.commit()
session.expunge_all()
print '-'*80

# check test data, delete one Test
t1 = session.query(Test).get(1)
print "t: ", t1
print "t.a: ", t1.audits
session.delete(t1)
session.commit()
session.expunge_all()
print '-'*80

# check that audits are still in the DB for deleted Test
t1 = session.query(Test).get(1)
assert t1 is None
_q = session.query(TestAuditLog).filter(TestAuditLog.entityId == 1)
_r = _q.all()
assert len(_r) == 2
for _a in _r:
    print _a
```


The other, more special case way is to set the relationship.passive_deletes flag to the string "all". This has the effect of entirely disabling SQLAlchemy’s behavior of setting the foreign key column to NULL, and a DELETE will be emitted for the parent row without any affect on the child row, even if the child row is present in memory. This may be desirable in the case when database-level foreign key triggers, either special ON DELETE settings or otherwise, need to be activated in all cases when a parent row is deleted.