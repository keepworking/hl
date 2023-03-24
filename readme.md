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

`-c` : change color

for example -c `green`, -cc `yellow`, -ccc `blue`, -cccc `magenta`, -ccccc `cyan`, -cccccc `strat on red again`

`-[0~5]` : set color
```
0 : red
1 : green
2 : yellow
3 : magenta
4 : cyan
````

`b` : set bright color

## example


```
adb logcat | hl -iE "fail|error"
```
![image](https://user-images.githubusercontent.com/14015519/227450360-095b996d-4f0a-4fa8-a317-89aa3ee9204e.png)
