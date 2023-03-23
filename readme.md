# hl : pipeline highlighter

simple pipeline highlight tool


## build

```
gcc hl.c -o hl
```

## install

```
chmod +x hl
sudo cp hl /usr/bin
```

## options

`-i` : ignore upper/lower case

`-E` : support extended regex

## example


```
adb logcat | hl -iE "fail|error"
```

![image](https://user-images.githubusercontent.com/14015519/227109188-6f13fb39-54e5-4a5a-9400-202b735a5051.png)
