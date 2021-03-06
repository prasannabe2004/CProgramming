<?php

    // configuration
    require("../includes/config.php"); 

    // if form is submitted
    if ($_SERVER["REQUEST_METHOD"] == "POST") 
    {
        if (preg_match("/^\d+$/", $_POST["deposit"]) == false)
        {
            apologize("You must enter a whole, positive integer.");
        }

        query("UPDATE users SET cash = cash + ? WHERE id = ?", $_POST["deposit"], $_SESSION["id"]);
        
        // redirect to portfolio 
        redirect("/");
    }
    
    // if form hasn't been submitted
    else
    {
        // render portfolio (pass in new portfolio table and cash)
        render("deposit_form.php", ["title" => "Deposit Form"]);
    }

?>
