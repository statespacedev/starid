# project utexas log

## 20250713 commit workflow1 - codename 'nice enough'

overll workflow has evolved and this is a good point to make it public as workflow1. things are working 'ok enough' to try moving onward to more topics, while keeping a watch out for ways to improve the following issues.

- tell command - 'tell at your own risk'
- pidp-10 statefullness -

sketch of workflow1

- step1 and step2, hardware - once tops10 boot disk is created, it's stable and doesn't need recreation. a possible gotcha here is the pidp-10 statefullness. occasional use of dec10test to 'clear the blinkers' and/or 'pulling the plug' may be needed, whenever things seem to be sideways, but seems that rebuilding the boot disk doesn't have any influence.
- step3 and step3, software - there's now a simh script boot-from-disk-fast.ini which goes directly to the tops10 prompt, without building decwar from tape. any prior decwar build should still be usable. the old boot-from-disk.ini is for building decwar.
- robots/start-some-robots.sh is for quickly bringing up a game with ten robots, for example. when robots are playing, the way to shut them all down is 'pkill -f python'. to watch the log from a robot use 'tail -f log1'.

## 20250511 first public mention of project utexas

first mention in github/drforbin/decwar discussions, 'project utexas - !call for testers!' - https://github.com/drforbin/decwar/discussions/27
