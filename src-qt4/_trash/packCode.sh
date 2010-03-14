#!/bin/bash
make clean
tar cfvzh iqr-`cat iqr.version`.tgz --exclude-from=excludepattern ../../iqr/src
