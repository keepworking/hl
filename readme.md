# hl : pipeline highlighter

simple pipeline highlight tool


## build

```
gcc hl.c -o hl
```

## install

```
chmod +ax hl
sudo cp hl /usr/bin
```

## options

`-i` : ignore upper/lower case

`-E` : support extended regex

## example


```
adb logcat | hl -iE "fail|error"
```

