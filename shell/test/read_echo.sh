#simple program to test pipes
#if you pass a number as argument it will call sleep (number)
read input_str;

if test $1; then
    sleep $1;
fi

echo $input_str;
