# $1 - bc/ll filename

# Generate PDFs with the CFG and DDG for the supplied LLVM IR.


SRC_FILE="$1"
CANONICALIZED_SRC_FILE="$1".format.cpp
SRC_FILE_BASENAME=`basename "$SRC_FILE"`
BENCHMARK="${SRC_FILE_BASENAME%.*}"
SRC_FILE_DIR=`dirname "$SRC_FILE"`

###
### STAGE 0: Get LLVM bitcode.
###
$LT_LLVM_INSTALL_DIR/build/bin/clang-format --style="{ColumnLimit: 2000, MaxEmptyLinesToKeep: 0}" \
                                            $SRC_FILE > $CANONICALIZED_SRC_FILE
./scripts/compilation/compile_to_bc.sh emu $CANONICALIZED_SRC_FILE
./scripts/compilation/prepare_ir.sh $CANONICALIZED_SRC_FILE.bc

BC_FILE=$CANONICALIZED_SRC_FILE.bc

# Standard DDG
$LT_LLVM_INSTALL_DIR/build/bin/opt -passes=dot-ddg $BC_FILE -o /dev/null > /dev/null 2>&1 
dot -Tpdf *.dot -o $SRC_FILE_DIR/DDG.pdf > /dev/null 2>&1 
echo $SRC_FILE_DIR/DDG.pdf
rm *.dot 

# DDG of only Strongly Connected Componennts
$LT_LLVM_INSTALL_DIR/build/bin/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libDDGDotPrinter.so \
                                   -passes=dot-ddg-sccs $BC_FILE > /dev/null 2>&1 

for fname in /tmp/DDG_function*.dot; do
  dot -Tpdf $fname -o $SRC_FILE_DIR/DDG_function.pdf
  echo $SRC_FILE_DIR/DDG_function.pdf
  rm $fname
done

i=0
for fname in /tmp/DDG_loop_*.dot; do
  dot -Tpdf $fname -o $SRC_FILE_DIR/DDG_loop_$i.pdf
  echo $SRC_FILE_DIR/DDG_loop_$i.pdf
  (( i++ ))
  rm $fname
done


# Multiple standard CFGs
$LT_LLVM_INSTALL_DIR/build/bin/opt -dot-cfg $BC_FILE > /dev/null 2>&1 
mv .*.dot $SRC_FILE_DIR
i=0
for fname in $SRC_FILE_DIR/.*.dot; do
  dot -Tpdf $fname -o $SRC_FILE_DIR/CFG_$i.pdf
  echo $SRC_FILE_DIR/CFG_$i.pdf
  (( i++ ))
done
rm $SRC_FILE_DIR/.*.dot


rm $CANONICALIZED_SRC_FILE*
