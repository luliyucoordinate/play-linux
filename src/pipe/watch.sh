
#! /bin/sh
who | sort > prev
while true ; do
    sleep 60
    who | sort > curr
    echo "logged out : "
    comm -23 prev curr
    echo "logged in : "
    comm -13 prev curr
    mv curr prev
done