for i in $(find . | grep png); do
    convert $i $i
done
