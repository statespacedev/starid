.PHONY: venv clean install

startvenv=. venv/bin/activate

venv:
	python3 -m venv venv/

clean:
	rm -rf venv

install:
	$(call startvenv) && pip install -r requirements.txt

reset: clean venv install