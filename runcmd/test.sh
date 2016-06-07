make clean
LOCAL_DIR=$HOME/Documents/5_sem/so2/jucilei/runcmd/local
#ls $LOCAL_DIR

make LDFLAGS="-L$LOCAL_DIR/lib -Wl,-rpath=/$LOCAL_DIR/lib" \
    CPPFLAGS="-I$LOCAL_DIR/include"
