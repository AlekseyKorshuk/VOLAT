cd visualizer
start python visualizer.py  --game_name "testVOLAT" --password "" --num_turns 45 --num_players 3 --hide_gui
cd ..

for file in `find ./cmake-build-debug/*.exe` ; do
    if [ -x $file ] ; then
        start $file "VOLAT1" "" "testVOLAT" "45" "3"
        start $file "VOLAT2" "" "testVOLAT" "45" "3"
        start $file "VOLAT3" "" "testVOLAT" "45" "3"
    fi
done
