from subprocess import call
import os

data = []

with open ("names.txt", "r") as myfile:
	for line in myfile:
		for word in line.split():
			data.append(word)

	for i in range (1,1000):
		call('./client '+ data[i], shell=True)