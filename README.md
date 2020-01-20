# old-code
Old projects to serve as a portfolio

This is a collection of code files from old classwork I did. I have lost many of my old files,
but I have included the files I think are significant that I still have access to.

--LoLScribe--
  The files prefixed with "ls" are part of a project I worked on in a course called Software
  Design and Documentation, where we went over some common software design and workflow
  techniques. As part of the course we were divided into groups and we designed, implemented,
  and documented a piece of software.
  
  My group created a tool meant to help players of a game called League of Legends (property of
  Riot Games) collate and analyze data about their performance in the game. I was primarily
  responsible for the data-gathering and analysis code, and the files I have included are the
  files that were written by me, with a bit of editing from my groupmates. Unfortunately it seems
  I did a terrible job of commenting that code, so it is not as readable as I would like, but I
  try my best to use descriptive variable names, so hopefully that will help assist the
  readability of the code.
  
  Our tool worked by using an open-source CURL API interface library to pull data from the simple
  API Riot Games had set up at the time and processing the raw data into useable statistics that
  we thought would be useful for players to analyze their performance in the game. I wrote
  several simple parsers in the main file to take the data we wanted from the raw files and
  write it out into much smaller data files for later use by the program. The other files were
  meant to work with data for individual players, primarily by adding additional data when an
  API pull found new games, or otherwise by loading and saving the data in simple files. There is
  also some code near the end of the main file that references other files that I have not
  included. Those were written by another group member and had to do with creating a GUI with
  another open-source library.
  
  Some time after we created this tool, Riot Games completely overhauled the API system for League
  of Legends, so our program no longer works, but I found it a very valuable project both from a
  personal and professional standpoint.
