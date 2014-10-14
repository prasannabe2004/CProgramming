<?php
    // configuration
    require("../includes/config.php");
    $rows =	query("SELECT * FROM portfolio WHERE id = ?", $_SESSION["id"]);
	$positions = [];
	foreach ($rows as $row)	
    {
        $stock = lookup($row["symbol"]);
        $stock["shares"] = $row["shares"];
        $stock["total"] = $row["shares"] * $stock["price"];
        $positions[] = $stock;
    }
    $users = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    render("portfolio.php", ["title" => "Portfolio", "portfolio" => $positions, "users" => $users]);
?>
