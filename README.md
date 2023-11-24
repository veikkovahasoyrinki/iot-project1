# iot-project1

IOT Course mini project 1 (521043S-3004) 

Juhana Moberg Y58151762, Veikko Vähäsöyrinki Y58172127, Eemil Kulmala 2308644 



## CLOUD: 

Run a python UDP server with Virtual Machine (Google)  

VM done with Google Compute: https://cloud.google.com/compute?hl=en 

Create VM Instance with default values and change stack type to Dual-stack(ipv4-ipv6) and add firewall rules: 

Open a ssh terminal from cloud console to the VM instance and run: 
``` ip a ```
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/8d3a4404-e79f-4586-bf65-6e15da82959d)

Save inet6 address from second interface, that is needed in next phase. Select a port f.ex 8683 (we used that) 

Make firewall rule to open egress and igress traffic for that ip address and port.

Ipv6 setting: https://cloud.google.com/compute/docs/ip-addresses/configure-ipv6-address 

Firewall rules: https://cloud.google.com/firewall/docs/firewalls 


UDP-server with python: 

We followed example: https://pythontic.com/modules/socket/udp-client-server-example 

Copy udp server python code to file and run (you need to add your own ip and port): 
```
touch udp-server.py
nano udp-server.py
python3 udp-server.py
```

## RIOT and FIT/Iotlab

Clone this repo and run: 
```
git clone https://github.com/veikkovahasoyrinki/iot-project1/
cd iot-project1
make all BOARD=iotlab-m3
cd RIOT/examples/gnrc_border_router
make all BOARD=iotlab-m3 ETHOS_BAUDRATE=500000
```

In FIT/Iotlab:
Set up SSH keys in FIT/Iotlab: https://iot-lab.github.io/docs/getting-started/ssh-access/
Create a new experiment with two nodes with M3 components with a chosen site, for example Grenoble.
Open 2 SSH terminals with: 
```
ssh <username>@<site>.iot-lab.info
```

In the other terminal:
```

```
