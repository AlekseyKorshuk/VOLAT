for file in `find ./cmake-build-debug/*.exe` ; do
    if [ -x $file ] ; then
        start $file "VOLAT1" "" "testVOLAT" "45" "3"
        start $file "VOLAT2" "" "testVOLAT" "45" "3"
        start $file "VOLAT3" "" "testVOLAT" "45" "3"
    fi
done
