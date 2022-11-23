<!DOCTYPE html>
<html>
	<head>
		<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Rubik">
		<title>WebServ</title>
		<style>
			body {
			display: flex;
			/* justify-content: center;
			align-items: center; */
			min-height: 100vh;
			background: radial-gradient(ellipse at bottom, #0d1d31 0%, #0c0d13 100%);
			overflow: hidden;
			font-family: 'Rubik';
			}
			h1, h2{
				margin-top: 10px;
				font-weight: bold;
			}
			h1{
				color: white;
				font-size: 65px;
				margin-top: 10px;
				margin-left: 10px;
			}
			h2{
				color: #607d8b;
				margin-top: -40px;
				font-size: 20px;
			}
			.head{
				margin-top: -10px;
			}
			a{
				color: white;
				font-size: 30px;
				display:block;
				height:100%;
				width:100%;
				border: 5px solid #607d8b;
				border-radius: 10px;
				/* background-color: white; */
				/* filter: blur(8px); */
				/* -webkit-filter: blur(8px); */
			}
			.stack{
				margin-top: 100px;
			}
			</style>
	</head>
	<body>
		<h1>Post with cgi</h1>
		<form method="POST" action="/methode/result_post.php">
			<label for="name">Nom</label>
			<input type="text" id="name" name="name">
			<label for="bd">Date de naissance</label>
			<input type="date" id="bd" name="bd">
			<input type="submit" value="Submit">
		</form>
	</body>
</html>
