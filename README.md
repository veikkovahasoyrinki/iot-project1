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
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/9b8cbbf1-bcbc-4647-a451-f9c80a8eea75)


Save inet6 address from second interface, that is needed in next phase. Select a port f.ex 8683 (we used that) 


Clone this git repo from the terminal
```
git clone https://github.com/veikkovahasoyrinki/iot-project1
```
Navigate to the repo and Change the IP address and port accordingly in the udp-server.py file with nano.
```
nano udp-server.py
```
Hit CTRL-X and Y to save and exit.

Next, start the server.
```
python3 udp-server.py
```

Next, clone the git repo to your local machine, navigate to the repo folder and test connectivity to the cloud server by running test_server_connection.py with. Modify the server's ip and port before running.

```
python3 test_server_connection.py
```
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
Navigate to the [FIT iotlab website](https://www.iot-lab.info/). Create an account by pressing "Access the testbed" -> "Register for an account".
Set up SSH keys by following instructions: [FIT/Iotlab](https://iot-lab.github.io/docs/getting-started/ssh-access/)

Next, log in to the website and Press "New experiment".

Use the default settings, press "Nodes" and "Node properties". Select m3 from architecture drop-down menu, Grenoble from "Site" and 2 from "Qty". Press add to experiment.

Press Submit experiment. Wait for your experiment to be in "running" state. Press the name of the id to access the experiment. 
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/71126486/f9edfa81-8bae-467c-8665-4c65ed912c50)

Choose one of the nodes to be the border router and press the "upload firmware" button in above screenshot. Click browse and select gnrc_border_router.elf file to it. Click flash.
Repeat the same steps for the other device, but select the "main" program .elf file instead. Now you have 2 boards, other one is acting as a border router and other one is acting as a iot device collecting data and sending it towards the cloud. The border router is your gateway to the internet. The "main" board is connected to the border router by being in the same network automatically.

Open one terminal from your local machine and open a SSH connection by 
```
ssh <username>@grenoble.iot-lab.info
```
Next, authenticate.
```
iotlab-auth -u <username>
```

Open a second terminal window and SSH that to the same site.

In one of the SSH terminals:

```
ip addr show | grep tap
```

Take tap that is free. E.g. if the above command prints tap1 interface, select tap2.

Remember the border router device id:
![image](https://github.com/veikkovahasoyrinki/iot-project1/assets/151513154/8765541b-a2a2-4007-9fe8-c6f682b641ed)


Run the below command, but fill your border routers id, free tap and ipv6 prefix of your site. (Grenoble ipv6 prefix : 2001:660:5307:3100)

```
sudo ethos_uhcpd.py m3-<id> tap<num> <ipv6_prefix>::/64
```
This terminal window connects to the border router board and becomes busy. You cannot type anything here. The border router is now configured.

Now in the other terminal that has the SSH connection and should be free, run below command and fill the "main" device's id.
```
nc m3-<id> 20000
```
Now you are connected to the "main" iot device with this terminal window. In here, run ifconfig and you should see that you have inet6 address with global scope. You will have to type the below commands as the shell is not advanced.
You can test connection by pingin a public Google server:

```
ping 2a00:1450:4007:80f::2003
```

Start sending, receiving and collecting data with: 

```
lsm start
```

Switch to the google VM cloud SSH window and see the messages coming in.
With udp_server.py code collected data is written to data.csv file with timestamp.
