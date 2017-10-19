# Exploiting a Stack-Based Buffer Overflow with No Mitigations

The buffer overflow is one of the most persistent and dangerous bugs in the history of computing, and while computers in 2017 have a few more protections than those in 1997, the underlying problem is still pretty much the same. By the end of this exercise, you will have successfully exploited a stack-based buffer overflow to gain a local shell on a modern 64-bit Linux system. 

## Setup the Environment
1. Open a 64-bit Linux environment. The Kali VM we've been using in VirtualBox will work perfectly. 
2. Ensure that your Linux environment can access the Internet. In VirtualBox, open Settings, go to Network, and select NAT. 
3. If Git is not installed, install it with "sudo apt install git", "sudo apt-get install git", or whatever if appropriate for your system. 
4. Install PEDA, a useful GDB extension (following commands lifted from the PEDA Readme)
```
git clone https://github.com/longld/peda.git ~/peda 
echo "source ~/peda/peda.py" >> ~/.gdbinit 
echo "DONE! debug your program with gdb and enjoy" 
```
4. Clone this repository locally 
```
git clone https://github.com/pittisc/overflow.git 
``` 
5. Navigate inside the respository 
```
cd overflow
```
6. Compile the vulnerable binary with stack canaries, NX, and relro disabled 
```
gcc -o vuln vuln.c -fno-stack-protector -z execstack -z norelro
``` 
7. NOTE! The next step disables a crucial security feature on your system. I'd recommend disconnecting that computer from the Internet (in VirtualBox, select "Host-Only" at the Network setting) and reenabling the feature before reconnecting it. 
8. Disable ASLR temporarily 
```
echo 0 > /proc/sys/kernel/randomize_va_space
```

## Reconnaissance
1. Python can be used to quickly generate input to the vulnerable binary. At the command line, execute a few commands using the -c flag to output various strings 
```
python -c 'print "A"*24' 
python -c 'print "AB"*10 + "C"*5' 
python -c 'print -c "\x41\x42\x43\x44"'
``` 
Take a look at the ASCII table with "man ascii" and ensure you undrstand why the last command gives the output it does. (Pay attention to the single and double quotes- it's important to use them as in the examples, with the single quotes on the outside of the expression and the double quotes on the inside surrounding strings)   
2. Execute the vulnerable program a few times and get a feel for its basic operation. Examine the contents of its source with cat and take a look at how it works under the hood. Specifically, look at how the program handles your input, and what restrictions are in place on that input (or aren't, but should be...) 
```
./vuln 
cat vuln.c 
```
3. Try sending the output of one of your Python commands directly to the input of the ./vuln binary by using the Bash construct $()  
```
./vuln $(python -c 'print "A"*24')
```
4. This program has a bug that causes it to accept and store inputs much larger than it has the ability to handle. Modify the Python command to send progressively larger inputs to the binary until you receive a "Segmentation Fault" error message.

## Exploitation
1. There's a Python file included in here. This file can exploit the vulnerable program using established methods of taking control of RIP and redirecting it to the beginning of the payload, which consists of a NOP slide down to shell-spawning shellcode. If execution is successful, you will receive a shell with the privileges of the user who compiled the program. 
2. Two things must be done to make this exploit functional. The offset must be changed to a value that will cause the payload to overwrite RIP with the retaddr, and a workable retaddr must be determined. There is only one right answer for the offset, but the retaddr can be any value that points to memory near the beginning (but probably not exactly at the beginning) of the NOP slide.
3. The debugger GDB can be used to obtain information about the program as it executes, and the PEDA extension will make GDB much easier to use. To load .\vuln into the debugger, just run
```
gdb ./vuln
```
4. You should now be greeted with a "gdb-peda$" prompt. Verify that all security mitigations are disabled with
```
checksec
```
5. PEDA's "pattern create" feature can be used to determine the proper offset to overwrite RIP. First, create a 512-byte pattern and save it to a file "arg" (or whatever you want to call it) 
```
pattern create 512 arg
```
6. Next, run the binary, pass the contents of "arg" as the input argument, and observe a crash 
```
run $(cat arg)

```
7. Use the corresponding "pattern search" feature to find the offset within the pattern that RSP, the stack pointer, currently points to. This is the offset that you will want to use to overwrite RIP. This is due to the fact that RIP segfaulted on a "ret" instruction, which would have placed the value at the top of the stack into RIP if that value had been a valid address. 
8. Finally, knowing that your payload is stored on the stack (this is a stack-based attack, after all) at a certain offset, calculate the address of the base of the string by subtracting the offset from the current value in the stack pointer, $RSP. 
9. Armed with this knowledge, you should be able to modify the included exploit template to exploit the vulnerability and spawn a shell. Have fun...   
