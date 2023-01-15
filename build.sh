#!/bin/bash

set -e

if [ $# != 1 ]; then
  echo 'got to provide the filename'
  exit
fi

NODIR=${1##*/}
FILENAME=${NODIR%.*}

echo "in:  $NODIR"
echo "out: $FILENAME"

# -MJ creates a file with compile config as JSON object, which is turned into
# the full compile_commands.json file with sed
COMPILE_COMMAND_SINGLE=compile_command_tmp.json
COMPILE_COMMAND_FULL=compile_commands.json

# better to use env for boost path???
BOOST_PATH=/home/kei/dev/cpp/boost_1_81_0/

clang++ -O0 -g -o bin/${FILENAME} \
  -I${BOOST_PATH} \
  -std=c++17 \
  -MJ${COMPILE_COMMAND_SINGLE} $1

# COMPILE_COMMAND_SINGLE ends with ',' which needs to be removed
cat ${COMPILE_COMMAND_SINGLE} | sed 's/^/[/' | sed 's/,$/]/' > $COMPILE_COMMAND_FULL


# nvim 
# :lua vim.cmd({cmd='!',args={'./build.sh', '%'}})
# :lua vim.keymap.set('n', '<leader>cc', function() vim.cmd({cmd='!',args={'./build.sh', '%'}}) end, { desc = 'run build.sh' })

