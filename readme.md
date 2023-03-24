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
![image](https://user-images.githubusercontent.com/14015519/227450360-095b996d-4f0a-4fa8-a317-89aa3ee9204e.png)
