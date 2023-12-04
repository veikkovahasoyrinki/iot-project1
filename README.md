# Iot mini project 1

In this project we wrote firmware for FIT/Iotlab-m3 board in RIOT OS which collects lsm303 acceleration data and sends it towards UDP-server which is hosted on Google Cloud Virtual Machine.

Juhana Moberg Y58151762, Veikko Vähäsöyrinki Y58172127, Eemil Kulmala 2308644 

[Video of working system](https://youtube.com/shorts/glUFfosewVg?feature=share)

First, you will need to set up the could:
## CLOUD: 
The could is a python UDP server running in Google cloud computing virtual machine instance.

Sign in with you Google credentials to [Google VM.](https://cloud.google.com/compute?hl=en) 

Set-up is free, however you will need to enter your card information to be able to create the VM. 

Enter your card credentials to gain access to the 300 dollars in free credits.

Open the side bar -> Compute engine -> VM instances -> Enable. Wait the Compute engine API to load.

Open the side bar and go to VPC network -> VPC networks -> Create new VPC network

Name your network, in the Subnet sections select custom creation mode to custom 

Select a region, remember this setting

Select IP stack type to IPV4 and IPv6 dual stack

Select IPv4 range to `10.0.0.0/24`

Leave the rest as default.

Press done in the subnet sections

In the firewall section, select all firewall settings to include with the VPC network from IPV4 and IPV6 tabs.

Press the name of the created VPC network.

Go to firewalls section. You need to create ingress and egress rules to allow IPv6 traffic.

Add firewall rule. Select an name and select your network. Select direction. Action on match: Allow

Targets: All instances in the network

Source filter: IPv6 ranges

Protocols and ports: Allow all. Leave the rest as default.

![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/242b02ea-1ed2-4b78-8286-87aa8f184dc6)

Create rules for ingress and engress traffic.

Now your VPC network details -> Firewalls should look like something like this:
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/087b3532-d86b-4e10-a5d0-3e3fe399dfbb)

Open the side bar -> Compute engine -> VM instances -> Create new instance. 

![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/61ea1cee-238e-425b-b172-85d1456786b0)

Select appropriate Region and Zone. Select the same Region as the created VPC network. In the Firewall section, allow HTTP and HTTPS traffic. Leave the rest of the settings as default. 

Go advanced options -> Networking -> Networking interfaces

Select your created VPC network

Change IP stack type to IPv4 and IPv6 dual-stack. Do not change the other settings. Press done.

Press Create.

Wait for the created instance to load.

![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/99580b35-61c9-48b4-87e2-821428085c84)


Open a ssh terminal from cloud console to the VM instance and run: 
``` ip a ```
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/8d3a4404-e79f-4586-bf65-6e15da82959d)

Save inet6 address from second interface, that is needed in next phase. Select a port f.ex 8683 (we used that) 


We followed example: [UDP server example](https://pythontic.com/modules/socket/udp-client-server-example) 

Copy udp server python code to file and run (you need to add your own ip and port): 
```
touch udp-server.py
nano udp-server.py
python3 udp-server.py
```

You can test connectivity to the cloud server by running test_server_connection.py with python. Change your address and port to that file.

## RIOT and FIT/Iotlab


Clone this repo and edit main.c to match your server's address and port (line 75) then run: 

```
git clone https://github.com/veikkovahasoyrinki/iot-project1/
cd iot-project1
make all BOARD=iotlab-m3
cd RIOT/examples/gnrc_border_router
make all BOARD=iotlab-m3 ETHOS_BAUDRATE=500000
```

In FIT/Iotlab:
Set up SSH keys in [FIT/Iotlab](https://iot-lab.github.io/docs/getting-started/ssh-access/)

Create a new experiment with two nodes with M3 components with a chosen site, for example Grenoble. 

Choose one of the nodes to be the border router and flash the gnrc_border_router.elf file to it. Flash main firmware to the other node. (.elf file)

Open two terminals and connect them via SSH to FIT/Iotlab. 
```
ssh <username>@<site>.iot-lab.info
```

In the other SSH terminal:

```
ip addr show | grep tap
```

Take tap that is free.
On below command fill your border routers id, free tap and ipv6 prefix of your site. (Grenoble ipv6 prefix : 2001:660:5307:3100)

```
sudo ethos_uhcpd.py m3-<id> tap<num> <ipv6_prefix>::/64
```

Now in the other terminal run: (id can be found from board name in Iotlab experiment)
```
nc m3-<id> 20000
```

Now run ifconfig and you should see that you have inet6 address with global scope.
You can test connection by pingin a pulbic Google server:

```
ping 2a00:1450:4007:80f::2003
```

Start sending, receiving and collecting data with: 

```
lsm start
```

With udp_server.py code collected data is written to data.csv file with timestamp.
