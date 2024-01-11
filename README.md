
# PNETLab Protocol Handler
Connects PNET's _vnc_, _capture_ and _telnet_ handlers to respective programs
<br>
The .reg files register an associated keyword (protocol) to do a certain action when opened from the windows explorer (or any browser). The **capture.reg** and **vnc.reg** redirect the url to the **pnet_wrapper.exe** app to parse the correct format for it's corresponding app. The **telnet.reg** redirects the url directly to telnet, as it's already capable of handling the full url string.

<br>
The capture of wireshark of a little more complicated. In a nutshell, because of the ssh tunneling, it can't directly set the interface and host for the connection via cli parameters. So it edits a profile-file of wireshark and replaces certain lines containing the config of the host and interface. And it then launches wireshark using the ssh-dump.exe interface and given ip.




### Pre-requirements
- [OpenSSH](https://learn.microsoft.com/en-us/windows-server/administration/openssh/openssh_install_firstuse?tabs=gui#install-openssh-for-windows) (for ssh-keygen to log in via wireshark. Only client is necessary, available via windows features)
- [Wireshark](https://www.wireshark.org/download.html) (for capturing network traffic)
- [PuTTY](https://www.putty.org/) (For telnet/ssh sessions)
- [UltraVNC](https://uvnc.com/downloads/ultravnc.html) (for vnc into linux machines)


### Installation
1. If not already, create a ssh key pair in default location (**%USERPROFILE%\\.ssh**). Copy your public key (content of **.ssh\id_rsa.pub**) to  *./.shh/authorized_keys* in the pnet virtual machine.

```
echo "<public key>" >> $HOME/.ssh/authorized_keys
```

2. Register respective _protocol handlers_ in windows registry. You can double click every .reg to add it to the registry.  **ALWAYS READ ANY .reg FILE BEFORE ADDING IT TO YOUR PC.**  

3. Copy the folder SSH into **%appdata%\Wireshark\profiles**
   
5. Copy the folder **pnet-wrapper** into **C:\\Program Files\\** (If you want another location, modify the .reg files accordingly)

## Be sure to disable html console. Now, selecting elements will trigger according apps in the host computer.
