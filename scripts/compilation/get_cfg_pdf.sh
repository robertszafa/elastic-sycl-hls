# $1 - bc/ll filename

SRC_FILE_DIR=`dirname "$1"`

$LT_LLVM_INSTALL_DIR/build/bin/opt -dot-cfg $1 > /dev/null 2>&1 

mv .*.dot $SRC_FILE_DIR

i=0
for fname in $SRC_FILE_DIR/.*.dot; do
  dot -Tpdf $fname -o $SRC_FILE_DIR/CFG_$i.pdf
  echo $SRC_FILE_DIR/CFG_$i.pdf
  (( i++ ))
done

rm $SRC_FILE_DIR/.*.dot

