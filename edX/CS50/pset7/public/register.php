 <?php
    // configuration
    require("../includes/config.php");
    if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if ($_POST["username"] == NULL) {
            apologize("You must enter a username");
        }
        else if ($_POST["password"] == NULL)
        {
            apologize("You must enter a password");
        }
        else if ($_POST["password"] != $_POST["confirmation"])
        {
            apologize("Your password and confirmation do not match");
        }
        $result = query("INSERT INTO users (username, hash, cash, email) VALUES(?, ?, 10000.00, ?)", $_POST["username"], crypt($_POST["password"]),$_POST["email"]);
        if($result === false)
        {
            apologize("Username already exists");
        }
        else
        {
            $rows = query("SELECT LAST_INSERT_ID() AS id");
            $id = $rows[0]["id"];
            $_SESSION["id"] = $id;
            redirect("index.php");
        }
    }
    else
    {
        render("register_form.php", ["title" => "Register"]);
    }
?>

