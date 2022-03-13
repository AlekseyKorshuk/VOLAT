#!/bin/bash
cd visualizer
python visualizer.py  --game_name "testVOLAT" --password "" --num_turns 99 --num_players 3 --hide_gui &
cd ..

if [[ "$OSTYPE" =~ ^msys ]]; then
    for file in $(find ./cmake-build-debug/*.exe) ; do
        if [ -x $file ] ; then
            start $file "VOLAT1" "" "testVOLAT" "99" "3"
            start $file "VOLAT2" "" "testVOLAT" "99" "3"
            start $file "VOLAT3" "" "testVOLAT" "99" "3"
        fi
    done
else
      for file in $(find ./cmake-build-debug/ -type f) ; do
          if [ -x $file ] ; then
              $file "VOLAT1" "" "testVOLAT" "99" "3" &
              $file "VOLAT2" "" "testVOLAT" "99" "3" &
              $file "VOLAT3" "" "testVOLAT" "99" "3" &
          fi
      done
fi



