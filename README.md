#485双向通讯程序#
##任务##
*	设计并制作一个如下图所示的有线双向多点数字通信装置。

##要求

1. 在此装置中主机上设有0—9数字按键，可以通过按键分别向1#—4#分别发送8位十进制数；
2. 1#—4#分机分别接有温度传感器，可测量液体0—100°C温度；
3. 要求每个分机都能够与主机单独进行通信；
4. 每个分机断电可在主机上有故障显示；
5. 每个分机有两路0—2.5V模拟量输入，并分别在主机上进行显示，主机具有能够记录分机数据的功能。
6. 1#—4#分机分别装有0—9按键，并可由一个分机向另外任何一个分机发送0—9的8位十进制数。
7. 分机采用3.6—3.7V电池供电，并要求电流小于10mA。
8. 分机要求有掉电保护功能。
9. 主机要求有打印功能。
10. 增加线路长度到200米以上。
	
##实现##
*	本程序基本实现了以上的功能(分机打印没有做),使用的是STC12C5A60S2(主机)和STC12LE5A60S2(从机)