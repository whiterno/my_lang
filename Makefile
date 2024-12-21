.PHONY: run

run:
	make frontend_run -C frontend
	make backend_run  -C backend
