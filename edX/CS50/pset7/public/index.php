<?php
    // configuration
    require("../includes/config.php");
    $rows =	query("SELECT * FROM portfolio WHERE id = ?", $_SESSION["id"]);
	$positions = [];
	foreach ($rows as $row)	
    {
        $stock = lookup($row["symbol"]);
        if($stock !== false)
        {
            $positions[] = [
                "name" => $stock["name"],
                "price" => $stock["price"],
                "shares" => $row["shares"],
                "symbol" => $row["symbol"],
                "total" => $row["shares"] * $stock["price"],
            ];
        }
    }
    $users = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    render("portfolio.php", ["title" => "Portfolio", "positions" => $positions, "users" => $users]);
?>
