<?php

    // configuration
    require("../includes/config.php"); 

    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["username"]))
        {
            apologize("Enter your username");
        }
        else if (empty($_POST["password"]))
        {
            apologize("Enter your password");
        }
        // find the username from DB
        $rows = query("SELECT * FROM users WHERE username = ?", $_POST["username"]);
        if (count($rows) === 1)
        {
            // User found from the DB
            $row = $rows[0];
            //Check the password matches
            if (crypt($_POST["password"], $row["hash"]) == $row["hash"])
            {
                $_SESSION["id"] = $row["id"];
                redirect("/index.php");
            }
            else
            {
                apologize("Sorry! Password not matched");
            }
        }
        else
        {
            // Zero user found from DB
            apologize("Invalid username and/or password.");
        }
    }
    else
    {
        render("login_form.php", ["title" => "Log In"]);
    }
?>
