
gcc -o hl hl.c
sudo cp hl /usr/bin


echo 'echo "red green yellow blue magenta cyan" | hl -0 "red" | hl -1 "green" | hl -2 "yellow" | hl -3 "blue" | hl -4 "magenta" | hl -5 "cyan"'
echo "red green yellow blue magenta cyan" | hl -0 "red" | hl -1 "green" | hl -2 "yellow" | hl -3 "blue" | hl -4 "magenta" | hl -5 "cyan"
echo 'echo "red green yellow blue magenta cyan" | hl -0b "red" | hl -1b "green" | hl -2b "yellow" | hl -3b "blue" | hl -4b "magenta" | hl -5b "cyan"'
echo "red green yellow blue magenta cyan" | hl -0b "red" | hl -1b "green" | hl -2b "yellow" | hl -3b "blue" | hl -4b "magenta" | hl -5b "cyan"
cat readme.md | hl -iE "fail|error" | hl -1b hl
