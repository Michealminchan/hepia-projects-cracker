for threadspow in {0..8};
do 
    let threads=$(echo 2^$threadspow|bc)
    FILE="res_${threads}.out"
    rm $FILE
    for i in {1..20}; 
    do
        ./main NO_HASH 12 $threads>>$FILE
    done
done