#!/bin/bash
set -e

message="Hello world!"

echo
echo "==> Compiling..."

pushd c > /dev/null
gcc -o bin/md5 md5.c -lm -DMD5_TEST
popd > /dev/null

pushd cpp > /dev/null
g++ -o bin/md5 md5.cpp -lm -DMD5_TEST
popd > /dev/null

pushd rust > /dev/null
cargo build --bin md5
popd > /dev/null

echo "==> Compilation complete."
echo

run_command() {
    local language="$1"
    local dir="$2"
    shift 2
    local cmd=( "$@" )
    pushd "$dir" > /dev/null
    local result
    result=$("${cmd[@]}" "$message")
    popd > /dev/null
    >&2 printf "%s - %s\n" "$result" "$language"
    echo "$result"
}

echo "==> Running..."
python_result=$(run_command "Python" "python" python main.py)
go_result=$(run_command "Go" "go" go run main.go)
rust_result=$(run_command "Rust" "rust" ./target/debug/md5)
c_result=$(run_command "C" "c" ./bin/md5)
cpp_result=$(run_command "C++" "cpp" ./bin/md5)
echo

if [[ "$python_result" == "$go_result" && "$go_result" == "$rust_result" && "$rust_result" == "$c_result" && "$c_result" == "$cpp_result" ]]; then
    echo "Generated hashes are identical: $python_result"
else
    results_arr=("$python_result" "$go_result" "$rust_result" "$c_result" "$cpp_result")
    languages_arr=("Python" "Go" "Rust" "C" "C++")
    found_inaccurate=false

    for i in "${!results_arr[@]}"; do
        count=0
        for r in "${results_arr[@]}"; do
            [[ "$r" == "${results_arr[i]}" ]] && count=$((count + 1))
        done
        if [ "$count" -eq 1 ]; then
            echo "${languages_arr[i]} result likely inaccurate: ${results_arr[i]}"
            found_inaccurate=true
            break
        fi
    done

    if ! $found_inaccurate; then
        echo "Multiple discrepancies found among results."
    fi
fi

echo

