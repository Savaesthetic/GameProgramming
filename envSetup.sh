# This file is meant to be sourced

# Argument check
if [ "$#" -ne 1 ]; then
    echo "Must pass a project to setup the environment for."
    return 2> /dev/null; exit
fi

# Setup the environment for the desired project
function setup()
{
    # Select the correct project from the argument
    if [ "$1" = "Test" ]; then
        export PROJECT_NAME="Test"
        export PROJECT_PATH=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )/${PROJECT_NAME}/
    elif [ "$1" = "SFML" ]; then
        export PROJECT_NAME="SFML"
        export PROJECT_PATH=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )/${PROJECT_NAME}/
    else
        echo "Error: Invalid project supplied."
        return 2> /dev/null; exit
    fi

    # Alias Setup
    local command_path="${PROJECT_PATH}projCmds.sh"
    alias build="${command_path} -b"
    alias config="${command_path} -c"
    alias clean="${command_path} -d"
    alias run="${command_path} -r"
}

# Run the setup function
setup $1