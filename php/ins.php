<?php
	$CMD="";
	if (isset($_GET['cmd']))
	{
		$CMD = $_GET['cmd'];
	}
	if (strlen($CMD) > 0)
	{
		//Mi connetto al MySql Server
		$myconn = mysql_connect('xxxxx', 'xxxxxx', 'xxxxxx') or die("Errore...");
		//echo "DB CONNESSO";
		//Mi connetto al database
		mysql_select_db('xxxxx', $myconn) or die("Errore...");
		// echo "DB SELEZIONATO";

		$query = "INSERT INTO val_pnl (cmd) VALUES ('" . $CMD . "')";

		
			
			$result = mysql_query($query, $myconn) or die("Errore...");
		echo "ok";
	}

?>
