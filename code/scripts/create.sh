make
insmod src/ flashcache
src/utils/flashcache_create -p thru -n <number of containers> -x <size of first container> -y <size of C2> -z <size of C3> -l <> -o <>  home /dev/sdb1 /dev/sda4
mount /dev/mapper/home-cached /shared
