# eh
Simple tool created for ELF binaries reverse engineering. Created during TUCTF2017 for task called unknown (re 200).

This tool is made for binaries with following control flow:  
``` c++
int checkCharacter(char c, int idx, ...maybe some other args){
  //some operations here (obfuscated or not)
  if(/*character at position idx should be c*/)
    return valueOk; 
  return valueError;
}

//somewhere in code

for(int i = 0; i < strlen(password); i++){
  if(checkCharacter(password[i], i, ...) == valueError)
    exit()
}
//validated!

```  

So, as this class of ciphers can be easily broken with simple bruteforce in   
O(password_length*charset) where charset = {azAZ1234567890!@#...} (all printable characters).

Using this tool requires you to make some simple reverse engineering as you must still provide address of first instruction after   `checkCharacter` call, address of some loop instruction, number of characters, valueError to avoid and register where result of `checkCharacter` is stored (usually rax). Such a tool isn't easy to abstract, so in special cases you will end up editing source code and recompiling to meet your expectations (for example `checkCharacter` might be `(c,i) -> isGood(c,i) ? i : -1`, so eh should "avoid" `i`).


##### Compilation:  
`g++ eh.cpp -o eh -std=c++11`   
##### Examples:
[simpl_xorme](examples/simple_xorme/README.md)  
[unknown tuctf2017](examples/tuctf2017_unknown_200/README.md)  


Have fun!  
