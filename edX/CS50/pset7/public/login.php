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
        $rows = query("SELECT * FROM users WHERE username = ?", $_POST["username"]);
        if (count($rows) == 1)
        {
            $row = $rows[0];
            if (crypt($_POST["password"], $row["hash"]) == $row["hash"])
            {
                $_SESSION["username"] = $row["username"];
                redirect("/index.php");
            }
            else
            {
                apologize("Sorry! Password not matched");
            }
        }
        else
        {
            apologize("Invalid username and/or password.");
        }
    }
    else
    {
        render("login_form.php", ["title" => "Log In"]);
    }
?>
