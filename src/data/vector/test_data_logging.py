
import logging
import sys
import time
from datetime import datetime
from logging.handlers import RotatingFileHandler
from pathlib import Path
import json
#  %(pathname)s:%(lineno)d
# '[%(asctime)s %(process)d] %(levelname)s %(name)s %(funcName)s %(message)s'

# base_dir = Path(__file__).parent
base_dir = Path("/tmp")
log_dir = base_dir / "tmpdata"
if not log_dir.exists():
    log_dir.mkdir(parents=True)
logfile = log_dir / "rotate_data.json"

rotate_hd = RotatingFileHandler(filename=logfile,
                                maxBytes= 3 * 2 ** 10,
                                backupCount=2)
myfmt = '%(message)s'
# myfmt = '[%(asctime)s %(process)d] %(levelname)s %(name)s %(funcName)s %(message)s'
formater = logging.Formatter(myfmt)
rotate_hd.setFormatter(formater)
# logger.addHandler(rotate_hd)
console_handler = logging.StreamHandler(stream=sys.stdout)
console_handler.setFormatter(formater)

logging.basicConfig(
                    level=logging.DEBUG,
                    #stream=sys.stdout, 
                    #filename="rotate_log.txt",  # 'stream' and 'filename' should not be specified together
                    handlers = [console_handler, rotate_hd],   # 'stream' or 'filename' should not be specified together with 'handlers'
                    # format='[%(asctime)s %(process)d] %(levelname)s %(name)s %(funcName)s %(message)s'
                    )


logger = logging.getLogger(__name__)


for i in range(1000):
    try:
        msg = {"i": i,
               "@timestamp": datetime.now().strftime("%Y-%m-%dT%H:%M:%S%z")}
        if i % 10 == 0:
            raise RuntimeError(i)
        # logger.info(f"i:{i} now:{datetime.now()}")
        logger.info(json.dumps(msg))
    except Exception as e:
        import traceback
        msg["exc"] = traceback.format_exc()
        logger.info(json.dumps(msg))
    finally:
        time.sleep(0.1)
