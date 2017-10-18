#Exploiting a Stack-Based Buffer Overflow with No Mitigations

The buffer overflow is one of the most persistent and dangerous bugs in the history of computing, and while computers in 2017 have a few more protections than those in 1997, the underlying problem is still pretty much the same. By the end of this exercise, you will have successfully exploited a stack-based buffer overflow to gain a local shell on a modern 64-bit Linux system. 

1. Open a 64-bit Linux environment. The Kali VM we've been using in VirtualBox will work perfectly. 
2. Ensure that your Linux environment can access the Internet. In VirtualBox, open Settings, go to Network, and select NAT. 
3. If Git is not installed, install it with "sudo apt install git", "sudo apt-get install git", or whatever if appropriate for your system. 
4. Clone this repository locally with "git clone LINK". 
5. Navigate inside the respository with "cd overflow". 
6. Compile the vulnerable binary with "gcc -o vuln vuln.c -fno-stack-protector -z execstack -z norelro". 
7. NOTE! The next step disables a crucial security feature on your system. I'd recommend disconnecting that computer from the Internet (in VirtualBox, select "Host-Only" at the Network setting) and reenabling the feature before reconnecting it. 
8. Disable ASLR with "echo 0 > /proc/sys/kernel/randomize_va_space". 
