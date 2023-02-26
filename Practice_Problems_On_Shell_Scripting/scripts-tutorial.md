 
Shell script files end with the extension .sh

"#" to comment  
"#!" is called SHEBANG.

# HOW TO EXECUTE

we will need to give execute permission with **`chmod +x script-name.sh`**  
then to run it, do **`./script-name.sh`**

<br>
<br>
<br>

# Variables

There is a variable called *USER*   
another called *PATH*

we can assign variable values like `x=1` and access them using `echo`

to take variable input, use the `read` command.  
this command reads input until enter is pressed  
it also truncates multiple whitespace characters into 1
the code 
```bash
read var
echo $var
```

```
 g       jhjh    hj
g jhjh hj
```
to not ignore multiple whitespace characters, we need to use quotes ("")
```bash
read var
echo "$var"
```
gives output
```
jhdaf   sdllfjha   dsaskfjhwqke
jhdaf   sdllfjha   dsaskfjhwqke
```
<br>
<br>

the code
```
x=$(ls)
```
will store the output of `ls` into the variable `x`  
if we then do `echo $x` we will get the full list of files and folders 
<br>
<br>
<br>

# Arithmatic

`1+1` will not work. 1 is not a command  
## expr command
we can use `expr` command  
`expr 1 + 1` will give `2`  
`expr 1+ 1` will not be able to do anything because the arguments have to be delimited by space

`expr 2 \* 5 + 3` will output 13  
`expr 10 / 4 - 1` will output 1  

## double parenthesis
this way, we don't have to worry about spacing. it works like normal integer expressions.  
but it will not automatically print the computed value.  

`echo $(((4+2*10)/ 5))` will output `4`  
\
`x=$(((4+2*10) / 5))` will store 4 into `x`  
`echo $x` will print 4


| Erroneous code | mistake|
| :-- | --: |
| `x=((3+1))`    | no `$` |
| `x = $((3+1))` | space  |

<br>
<br>
<br>

# Conditionals
```bash 
if [ condition ]; then
    statement
else
```
***IMPORTANT*** : the condition inside the square brackets needs to have space between them

| correct | wrong |
| :--: | :--: |
| `[ condition ]` | `[condition]` |
||`[ condition]`|
||`[condition ]`|

### Code for comparing strings
```bash
#!/bin/bash

echo "guess the string "
read input
echo your input is $input
expected_input="xyz"
if [ $input = $expected_input ]; then
    echo "You guessed right!"
else 
    echo "Wrong!"
fi
```
 - for not equals, use `string1 != string2`
 - for not null, use `-n string`
 - for null, use `-z string`

### SOMETHING I NOTICED 
if the line 
```bash
if [ $input = $expected_input ]; then
```
is changed to 
```bash
if [ $input=$expected_input ]; then
```
then it becomes true.
I guess the spaces before and after `=` are really important in differenciating from **equality** and **assignment**

## Comparing two integers
if we want to compare integers we should use the following
 - -eq for *equals*
 - -ne for *not equals*
 - -gt for *greater than*
 - -ge for *greater or equals*
 - -lt for *less than*
 - -le for *less or equals* 
 
but the easiest way to compare is to use `(())` double parenthesis.  
that way we can compare integers using the conventional `>` `<` `>=` symbols

## Difference between `[[]]` and `[]`
we can surround the conditions in two ways
```bash
if [[ $val = "abc"]]; then
```
```bash
if [ $val = "abc"]; then
```

the first way provides more functionality. such as
- if we want to `&&` multiple conditions in the two ways
    - `[[ cond1 && cond2 ]]`
    - `[cond1 -a cond2]`  
the first way is more suitable for us  

more info [here](http://mywiki.wooledge.org/BashFAQ/031)  

## The syntax `if command`
Every command has an *exit status*. The value 0 denotes that the command has exited successfully.  
otherwise the exit status will be 1  
***NOTE*** to access the exit status of a command, do  
`echo $?`  
after executing the command.

```bash
if grep i text.txt; then
    if-block
fi
```
this condition will evaluate to `true` when the command has indeed found the pattern *i* in the *text.txt* file and execute the **if-block**

## Short circuiting code
```bash
condition && echo "cond true hole eta print"
```
```bash
condition || echo "cond false hole eta print"
```
<br>
the below code exits if the number of arguments is not equal to 1  

```bash
[[ $# != 1 ]] && exit 1
```
<br>
<br>  

***NOTE***  
```bash
(( $# != 1 )) && exit 1
```
and
```bash
[[ $# -ne 1 ]] && exit 1
```
works too  
<br>
### doing multiple things if condition is true  

```bash
(( $# != 1 )) && {
    echo "needs 1 argument. Arguments provided is " $# 
    exit 1
}

echo got $# argument and it is $1
```
<br>
<br>
<br>

# FOR loop
a for loop to test which files in the current directory is actually a file
```bash
#!/bin/bash

# isfile 

for file in $(ls); do
    if [ -f $file ]; then
        echo $file " is a file"
    else
        echo $file " is not a file"
    fi
done
```

we can also create the range of numbers manually for the for loop
```bash
for i in {1..5}; do
    echo $i
done
```

<br>
<br>
<br>

# While loop
```bash
#!/bin/bash

password="abcd"
echo "Enter Password : "
read input

while [ $input != $password ] 
do
    echo "Wrong Password. Try again"
    read input
done
echo "CORRECT"
```
# Command substitution
```bash
x=$(ls)
```
is the same as 
```bash
x=`ls`
```
*but using $(ls) is preferable because this way you can nest commands together.*  

<br>
<br>
<br>

# Giving arguments in a script
```
./sum.sh 12 33 14
```
we want sum.sh to add the argument numbers
```bash
#!/bin/bash

echo "number of arguments is $#"
sum=0
for arg in $*; do
    sum=$(($sum+$arg))
done
echo "sum is" $sum
```
<br>
<br>
<br>

# Functions
the syntax is 
```bash
func-name() {
    statements
}
```
notice that there are no arguments in the function declaration.  
instead we pass the arguments when we call the function by making them space seperated

```bash
greet(){
    echo "hello there, $1"
}

greet "razin reaz"
echo $1
```
in the above code, the `$1` in the `greet` function corresponds to the first argument passed to the `greet` function.  
the `echo $1` corresponds to the first arguments passed when we run the script in the command line  

## Return
using `return` is not recommended because it can only return 1 byte.
use `echo` instead
<br>
<br>
**NOTE**  
we can do
```bash
x=$(function arg)
```
apparently this technique is very powerful
<br>
<br>
<br>

# Recursion
below is a recursive code to calculate factorial
```bash
#!/bin/bash
#argument has to be passed from the cmd
fact(){
    if (( $1 == 0 )); then
        echo 1
        return
    fi
    last=$(fact $(($1 - 1)))
    echo $(( $last * $1 ))
}

fact $1
```
<br>
<br>
<br>

# ARRAYS
### Declaration
the elements have to be ***space-seperated*** and eclosed by ***( )***
### Accessing elements
The syntax is different  
`echo ${a[$i]}` will print `a[i]`  

```bash
# Declaration
a=(1 2 4 15)

sum=0
# For each loop
for x in ${a[@]}; do
    sum=$(($sum+$x))
    echo current sum is $sum
done

echo ${a[1]}

# Access using indices
for i in ${!a[@]}; do
    echo element $i is ${a[$i]}
done
```
output
```
current sum is 1
current sum is 3
current sum is 7
current sum is 22
2
element 0 is 1
element 1 is 2
element 2 is 4
element 3 is 15
```

<br>
<br>
<br>

# COMMON MISTAKES
- when assigning a variable, putting space surrounding the `=` operator
- not putting `$` before `(())` 