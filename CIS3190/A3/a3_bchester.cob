*> CIS 3190 Assignment 3: Text Analyzer
*> Author - Brandon Chester. 0877477

*> See included README.txt for instructions on compilation + usage + limitations

identification division.
program-id. a3.

environment division.

input-output section.
file-control.

select inputFile assign to dynamic inputFilename
    organization is line sequential.

select outputFile assign to "TextAnalysis.txt"
    organization is line sequential.

data division.

file section.
fd inputFile.
01 sample-input pic x(80).

fd outputFile.
01 outputLine pic x(80).

working-storage section.
77 endOfFileFlag pic 9.
77 inputFilename pic x(30).

01 numberOfSentences  pic 9(4) comp.
01 numberOfWords      pic 9(4) comp.
01 numberOfNumbers    pic 9(4) comp.
01 numberOfCharacters pic 9(4) comp.

01 currentIndex  pic 9(2) comp.
01 isWordChunk   pic 9 comp.
01 isNumberChunk pic 9 comp.

01 inputArea.
    02 inputAreaLine pic x occurs 80 times.

01 outputFileTitleLine.
    02 filler pic x(31) value spaces.
    02 filler pic x(19) value "Input text analyzed".

01 outputUnderline.
    02 filler pic x(40)
        value "----------------------------------------".
    02 filler pic x(40)
        value "----------------------------------------".

01 sentenceStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(21) value "Number of sentences =".
    02 numberOfSentencesOutput pic -(2)9.

01 wordStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(17) value "Number of words =".
    02 numberOfWordsOutput pic -(4)9.

01 numberStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(19) value "Number of numbers =".
    02 numberOfNumbersOutput pic -(4)9.

01 characterStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(22) value "Number of characters =".
    02 numberOfCharactersOutput pic -(4)9.

01 averageWordStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(38) value "Average number of words per sentence =".
    02 averageWordsPerSentence pic -(4)9.9(2).

01 averageSymbolStatistics.
    02 filler pic x(10) value spaces.
    02 filler pic x(39) value "Average number of characters per word =".
    02 averageCharactersPerWord pic -(4)9.9(2).

procedure division.
    *> Grab input file from the user
    display "Enter the name of the file to be analyzed:".
    accept inputFilename.

    open input inputFile, output outputFile.
    *> Put the header in the output file
    write outputLine from outputFileTitleLine after advancing 0 lines.
    write outputLine from outputUnderline after advancing 1 line.

    *> Begin the core program tasks
    perform ReadInputFile until endOfFileFlag = 1.

    *> Reads the input file and calls a function to analyze each line
    ReadInputFile.
        *> Initialize counter variables
        move 0 to numberOfNumbers
        move 0 to numberOfWords
        move 0 to numberOfSentences
        move 0 to endOfFileFlag
        *> Read lines until end of file is found
        perform until endOfFileFlag = 1
            read inputFile into inputArea
                at end
                    move 1 to endOfFileFlag
                    perform OutputFileStatistics
                not at end
                    *> Write lines to output file as they are read and analyzed
                    write outputLine from inputArea after advancing 1 line
                    move 0 to currentIndex
                    move 0 to isWordChunk
                    move 0 to isNumberChunk
                    perform AnalyzeLine until currentIndex = 80
            end-read
        end-perform.

    *> Looks at a line of input character by character to count the number of words, numbers, letters, and sentences.
    AnalyzeLine.
        add 1 to currentIndex
        *> For this program, a character is only one of the 26 English letters
        if inputAreaLine(currentIndex) is alphabetic
            *> Cobol includes spaces as alphabetic because ?
            if inputAreaLine(currentIndex) not = " "
                add 1 to numberOfCharacters
            end-if
        end-if.

        *> Program can recognize sentences ending in period, question mark, exclamation mark
        if inputAreaLine(currentIndex) = "." or inputAreaLine(currentIndex) = "?" or inputAreaLine(currentIndex) = "!"
            add 1 to numberOfSentences
        end-if.

        *> If currently in a recognized chunk of alphabetic characters
        if isWordChunk = 1
            *> If we hit a space, the word is finished
            if inputAreaLine(currentIndex) = " "
                move 0 to isWordChunk
            end-if
        *> If current in a chunk of numeric characters
        else if isNumberChunk = 1
            *> If hit a space, end of number
            if inputAreaLine(currentIndex) = " "
                move 0 to isNumberChunk
            end-if
        else
            *> if the character is numeric, we have a new number
            if inputAreaLine(currentIndex) is numeric
                move 1 to isNumberChunk
                add 1 to numberOfNumbers
            *> If the character is not a space or a hyphen separator, we have a new word
            else if inputAreaLine(currentIndex) not = " " and inputAreaLine(currentIndex) not = "-"
                move 1 to isWordChunk
                add 1 to numberOfWords
            end-if
        end-if.

    *> Outputs the various statistics that the program collected while reading the file
    OutputFileStatistics.
        move numberOfSentences to numberOfSentencesOutput.
        move numberOfWords to numberOfWordsOutput.
        move numberOfCharacters to numberOfCharactersOutput.
        move numberOfNumbers to numberOfNumbersOutput.

        compute averageWordsPerSentence rounded = numberOfWords/numberOfSentences.
        compute averageCharactersPerWord rounded = numberOfCharacters/numberOfWords.

        write outputLine from outputUnderline after advancing 1 line.
        write outputLine from sentenceStatistics after advancing 1 line.
        write outputLine from wordStatistics after advancing 1 line.
        write outputLine from numberStatistics after advancing 1 line.
        write outputLine from characterStatistics after advancing 1 line.
        write outputLine from averageWordStatistics after advancing 1 line.
        write outputLine from averageSymbolStatistics after advancing 1 line.
        write outputLine from outputUnderline after advancing 1 line.
        perform EndOfProgram.

    EndOfProgram.
        close inputFile.
        close outputFile.
        stop run.








