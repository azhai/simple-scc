#!/bin/sh

set -e

cd files
ar -qv ../test.a file1 file2
ar -qv ../test.a file3
