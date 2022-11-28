<?php

if (!session_id())
{
	session_start();
	session_regenerate_id(true);
	$_SESSION["session"] = "new";
}

?>

<br><br>
<br><br>

<!DOCTYPE html>
<html lang="fr">
	<head>
		<center>
			<div class="text">
					<h1><a href="/">
						<button class="btn btn-primary w3-black w3-padding-large w3-large w3-margin-top"> Back to Index </button>
					</a></h1>
					<div class="text">
					>>Session has been created<<</p>
					<h1><a href="/session/session_display.php">
						<button class="btn btn-primary w3-red w3-padding-large w3-large w3-margin-top"> Session Display </button>
					</a></h1>
					<h1><a href="/session/set_session.php">
						<button class="btn btn-primary w3-red w3-padding-large w3-large w3-margin-top"> Set Session </button>
					</a></h1>
			<div class="text">
				<h1><a href="/session/session_deconnexion.php">
				<button class="btn btn-primary w3-black w3-padding-large w3-large w3-margin-top"> Log out </button>
				</a></h1>
		</center>
		<title>BONUS SESSION</title>
		<meta charset="UTF-8">

	</head>

</html>