echo "example1"
./anon ../examples/example1 > ./tests/output/example1.out
#diff ./tests/output/example1.out ./tests/expected_cache/example1.out
echo "Done."

echo "example2"
./anon ../examples/example2 > ./tests/output/example2.out
#diff ./tests/output/example2.out ./tests/expected_cache/example2.out
echo "Done."

echo "example3"
./anon ../examples/example3 > ./tests/output/example3.out
#diff ./tests/output/example3.out ./tests/expected_cache/example3.out
echo "Done."

echo "example4"
./anon ../examples/example4 > ./tests/output/example4.out
#diff ./tests/output/example4.out ./tests/expected_cache/example4.out
echo "Done."

echo "example5"
./anon ../examples/example5 > ./tests/output/example5.out
#diff ./tests/output/example5.out ./tests/expected_cache/example5.out
echo "Done."

echo "example6"
./anon ../examples/example6 > ./tests/output/example6.out
#diff ./tests/output/example6.out ./tests/expected_cache/example6.out
echo "Done."

echo "example7"
./anon ../examples/example7 > ./tests/output/example7.out
#diff ./tests/output/example7.out ./tests/expected_cache/example7.out
echo "Done."

echo "sms"
./anon ../../../results/dataset_final/nauty_input2/sms.csv.nauty > ./tests/output/copnet-sms.out
#diff ./tests/output/copnet-sms.out ./tests/expected_cache/copnet-sms.out