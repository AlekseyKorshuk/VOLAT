#!/bin/bash
cd visualizer
python visualizer.py  --game_name "testVOLAT2" --password "" --num_turns 99 --num_players 3 --hide_gui &
cd ..

if [[ "$OSTYPE" =~ ^msys ]]; then
    for file in $(find ./cmake-build-debug/*.exe) ; do
        if [ -x $file ] ; then
            $file "VOLAT1" "" "testVOLAT2" "99" "3" "0"
        fi
    done
else
      for file in $(find ./cmake-build-debug/ -type f) ; do
          if [ -x $file ] ; then
              $file "VOLAT1" "" "testVOLAT2" "99" "3" "0"
          fi
      done
fi



