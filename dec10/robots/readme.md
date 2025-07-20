# utexas robots

in your utexas folder, have a venv python virtual environment. can create that with

    python -m venv venv

have your bash shell set to use your venv

    source venv/bin/activate

your bash prompt should now begin with (venv) showing it's active. within your venv, have pip installed pexpect and sshkeyboard

    pip install pexpect
    pip install sshkeyboard

with project utexas 'booted from disk' and running in a terminal, open other terminals and use the commands

    python robots/noninteractive.py somename
    python robots/interactive.py somename
    bash robots/start-some-robots.sh
