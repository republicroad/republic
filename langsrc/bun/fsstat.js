
// https://stackoverflow.com/questions/32705219/nodejs-accessing-file-with-relative-path
// https://github.com/oven-sh/bun/issues/3167

import {open} from 'node:fs/promises';
var path = require('path');

// var jsonPath = path.join(__dirname, '..', 'config', 'dev', 'foobar.json');
// relative current file directory: __dirname
var filepath = path.join(__dirname, 'readme.md');
console.log(filepath);

const file = await open(filepath, 'r');
const stat = await file.stat();
console.log(stat);
console.log(stat.size);