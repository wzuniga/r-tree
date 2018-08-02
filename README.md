# R-tree
r-tree implementation in python and c++ core.

## Getting Started
To run the project we need to install some python packages.

### Prerequisites

First be sure, you have installed python3.
```
sudo apt-get install python3.6
```

You need to create a new directory and entrer there.
```
mkdir r-tree
cd r-tree
```

Now we need a virtualenv to avoid version problems
```
sudo apt-get install python3-venv
python3 -m venv myworkvenv
source myworkvenv/bin/activate
```

Now we'll install django, the server choosed
```
pip install django==1.8
```

esto nos mostrará un mensaje como el siguiente
```
(myworkvenv) ~$ pip install django==1.8
Downloading/unpacking django==1.8
Installing collected packages: django
Successfully installed django
Cleaning up...
```

avoid these instructions
```
django-admin.py startproject WebApplication .
python manage.py startapp blog
```

Here clone the project github and run on terminal
```
python manage.py migrate
python manage.py runserverls

```

Look your site on
```
http://127.0.0.1:8000/
```

