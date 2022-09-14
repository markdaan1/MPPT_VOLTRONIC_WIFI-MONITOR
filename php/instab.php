<?php

//Mi connetto al MySql Server
$myconn = mysqli_connect('xxxxxx', 'xxxxxxx', 'xxxxxx','xxxxxxx') or die('Errore...');
//echo "DB CONNESSO";

$query = "SELECT * from val_pnl";

//$result = mysql_query($query, $myconn) or die("Errore...");
$result=mysqli_query($myconn,$query); 

/*
00 VAC   
01 HzAC    
02 VOUT     
03 HzOUT     
04 APPARENT    
05 WASSORBITI
06 NC
07 NC
08 VBATT
09 ChgA
10 BATTPERCENTO
11 TMP_INVERTER	
12 PVA
13 PVV
14 NC
15 DISCHARGE_A
16 STATOCHG
17 NC
18 NC  
19 PV_W
 */
 
$VoltAC=0;
$FreqAC=1;
$VoltOUT=2;
$FreqOUT=3;
  
$Watt=5;
  
$VoltBatt=8;
$ChgCurr=9; 
$BattLVLPercent=10;
$VoltPNL=13;   
  
$DiscCurr=15;  
  
if ($result)
{
	$numrows = mysqli_num_rows($result);
	for ($x=0;$x<$numrows;$x++)
	{
		$fieldinfo=mysqli_fetch_array($result);
		$TS=$fieldinfo['timestamp'];
		$ValLetto=$fieldinfo['cmd'];
		
		$VALORI = explode("X", $ValLetto);
		$VALORI_TMP = explode("_", $VALORI[1]);
		
		echo $TS . " -> " . $VALORI_TMP[$VoltAC] . " " . $VALORI_TMP[$FreqAC] . " " . $VALORI_TMP[$VoltOUT] . " " . $VALORI_TMP[$FreqOUT] . " <b>" . $VALORI_TMP[$Watt] . " " . $VALORI_TMP[$BattLVLPercent] . " " .  $VALORI_TMP[$VoltBatt] . "</b> " . $VALORI_TMP[$DiscCurr] .  "<br>"; // piece1
		 
		
	}
} 




?>
