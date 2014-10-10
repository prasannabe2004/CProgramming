<?php
    
    //configuration
    require("../includes/config.php");
    
    
    //if form was submitted
    if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["confirmation"]) || empty($_POST["password"]))
        {
            apologize("confirmation or password cannot be empty!");
        }
        if(($_POST["password"] != $_POST["confirmation"]))
        {
            apologize("Password do not match!");
        }
        if (empty($_POST["username"]) )
        {
            apologize("username cannot be empty!");
        }        
        else
        {
            $result = query("INSERT INTO users (username,hash,cash) VALUES(?,?,10000.00)", $_POST["username"], crypt($_POST["password"]));
            if($result === false)
            {
                printf("query fails \n");
            }
            else
            {
                $rows = query("SELECT LAST_INSERT_ID() AS id");
                $id = $rows[0]["id"];
                $_SESSION["id"] = $id;
                // Redirect to index.html
                printf("success");
                redirect("/index.php");
            }
            
        }
    }
    else
    {
        //else render form
        render("register_form.php", ["title" => "Register"]);
    }
?>
