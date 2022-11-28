<?php
	session_start();
?>

<br></br>
<br></br>

<!DOCTYPE html>
<html>

<head>
	<center>
		<div class="text">
            <h1><a href="/session/session_start.php">
				<button class="btn btn-primary w3-black w3-padding-large w3-large w3-margin-top"> Back to Session start </button>
			</a></h1>
	</center>

	<title>BONUS SESSION</title>
	<meta charset="UTF-8">

</head>
	
<body>
<center>
	<form method="post">
			<label for="fname">First name:</label>
			<br></br>
			<input type="text" id="fname" name="fname"><br></br>
			<label for="lname">Last name:</label>
			<br></br>
			<input type="text" id="lname" name="lname"><br></br>
			<input type="submit" value="Submit">
	</form>
</center>		
</body>

<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
  // collect value of input field
	$_SESSION[$_POST['fname']] = $_POST["fname"];
	$_SESSION[$_POST['lname']] = $_POST['lname'];

	echo "<center>";
	echo "<br>";
	if (empty($_POST['fname'])) {
	echo "\"First Name\" is empty";
	echo "<br>";
	} else {
	echo $_POST['fname'];
	echo " --> is set";
	echo "<br>";
	}
	if (empty($_POST['lname'])) {
    echo "\"Last Name\" is empty";
	} else {
	echo $_POST['lname'];
	echo " --> is set";
	echo "<br>";
	echo "</center>";
  }
}
?>

</body>
</html>
