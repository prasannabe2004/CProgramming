<?php
    
    // configuration 
    require("../includes/config.php");   
    
    // if form is submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["symbol"]) || empty($_POST["shares"]))
        {
            apologize("You must enter a stock symbol and quantity of shares to buy.");
        }
        $stock = lookup($_POST["symbol"]);
        if ($stock === false)
        {
            apologize("Invalid stock symbol.");
        }
        if (preg_match("/^\d+$/", $_POST["shares"]) == false)
        {
            apologize("You must enter a whole, positive integer.");
        }
        $transaction = 'BUY';
        $cost = $stock["price"] * $_POST["shares"];
        $cash =	query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
        // thanks to dump to figure out the comparision
        if ($cash[0]["cash"] < $cost)
        {
            apologize("No enough cash to buy this share at this time. Please deposit cash.");
        }
        else
        {
            $_POST["symbol"] = strtoupper($_POST["symbol"]);
            query("UPDATE users SET cash = cash - ? WHERE id = ?", $cost, $_SESSION["id"]);
            query("INSERT INTO portfolio (id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $_SESSION["id"], $_POST["symbol"], $_POST["shares"]);
            query("INSERT INTO history (id, transaction, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", $_SESSION["id"], $transaction, $_POST["symbol"], $_POST["shares"], $stock["price"]);
            redirect("/");    
        }
    }
    else
    {
        render("buy_form.php", ["title" => "Buy Form"]);
    }
    
?>
