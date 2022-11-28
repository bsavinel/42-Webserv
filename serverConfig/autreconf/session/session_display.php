<?php
    session_start();
    // echo "<h3> PHP List All Session Variables</h3>";
    // foreach ($_SESSION as $key=>$val)
    // echo $key." ".$val."<br/>";
	echo "<br></br>";
	echo "<br></br>";
	echo "<center>";
	echo '<pre>' . print_r($_SESSION, TRUE) . '</pre>';
	echo "</center>";

?>
<html>
	<head>
		<title>BONUS SESSION</title>
		<meta charset="UTF-8">
</head>
	</head>
<body>
	<center>
	<div class="text">
            <h1><a href="/session/session_start.php">
			<button class="btn btn-primary w3-black w3-padding-large w3-large w3-margin-top"> Back to Session start </button>
			</a></h1>
	</div>
	</center>

</body>


</html>