import os

# Define constants TODO: Set from commandline args
inputFileName = "log.csv"
outputFileName = "chart.csv"
associationWindow = 100 #In ms

# Global vars
tsBuffer = list()
ledNameBuffer = list()
hitMatrix = {}  # key is LED name, value is dictionary.
                # the subdictionary key is the other LED name, and v is associated hit count

# Set working directory
workingDir = os.path.dirname(os.path.abspath(__file__))
os.chdir(workingDir)

def read_reverse_order(file_name):
    # Open file for reading in binary mode
    with open(file_name, 'rb') as read_obj:
        # Move the cursor to the end of the file
        read_obj.seek(0, os.SEEK_END)
        # Get the current position of pointer i.e eof
        pointer_location = read_obj.tell()
        # Create a buffer to keep the last read line
        buffer = bytearray()
        # Loop till pointer reaches the top of the file
        while pointer_location >= 0:
            # Move the file pointer to the location pointed by pointer_location
            read_obj.seek(pointer_location)
            # Shift pointer location by -1
            pointer_location = pointer_location -1
            # read that byte / character
            new_byte = read_obj.read(1)
            # If the read byte is new line character then it means one line is read
            if new_byte == b'\n':
                # Fetch the line from buffer and yield it
                yield buffer.decode()[::-1]
                # Reinitialize the byte array to save next line
                buffer = bytearray()
            else:
                # If last read character is not eol then add it in buffer
                buffer.extend(new_byte)
        # As file is read completely, if there is still data in buffer, then its the first line.
        if len(buffer) > 0:
            # Yield the first line too
            yield buffer.decode()[::-1]

# Formats the timestamp into an integer
#  good for comparison
def formatTimeStamp(stamp):
    tokens = stamp.split(":")
    returnVal = int(tokens[0]) * 60 * 60 * 1000
    returnVal += int(tokens[1]) * 60 * 1000
    returnVal += int(tokens[2]) * 1000
    returnVal += int(tokens[3])
    return returnVal

# Populates hit matrix in a single direction
def analyzeLogLine(line):
    if line == '':
        return None
    
    tokens = line.split(",")
    if tokens[2] == "LED On" or tokens[2] == "LED Off":
        tsBuffer.append(formatTimeStamp(tokens[1]))
        ledNameBuffer.append(tokens[3].rstrip())

        # Ensure that buffer only contains recent events
        distance = abs(tsBuffer[-1] - tsBuffer[0])
        
        # TODO: Handle time wraparound at midnight.
        # This code was removed as it doesn't work in reverse order.
        # if distance < 0:
        #     distance += 1000 * 60 * 60 * 60 * 24
        
        while distance > associationWindow:
            tsBuffer.pop(0)
            ledNameBuffer.pop(0)
            distance = abs(tsBuffer[-1] - tsBuffer[0])
            # if distance < 0: # handle wraparound at midnight
            #     distance += 1000 * 60 * 60 * 60 * 24

        # Add a dictionary for this LED if it doesn't exist
        if not (tokens[3].rstrip() in hitMatrix.keys()):
            hitMatrix[tokens[3].rstrip()] = {}

        # Add hits
        for i in ledNameBuffer:
            if (i == tokens[3].rstrip()):
                continue # avoid self-hits
            else:
                if i in hitMatrix[tokens[3].rstrip()].keys():
                    # previous association present
                    hitMatrix[tokens[3].rstrip()][i] += 1
                else:
                    # this is the first association
                    hitMatrix[tokens[3].rstrip()][i] = 1

def main():
    try:
        infile = open(inputFileName, 'rt')
        outfile = open(outputFileName, 'wt')
    except OSError as e:
        if (e.filename == inputFileName):
            print(f"Issue opening log file.  Error: {e.args[1]}.  Aborting script.")
        elif (e.filename == outputFileName):
            print(f"Issue opening output file.  Error: {e.args[1]}.  Aborting script.")
            infile.close()
    else:
        print ("Generating table:")

        # Identify associations within the given window
        # Iteration must be done both directions to catch associations both before and after an event.
        for line in infile:
            analyzeLogLine(line)
        infile.close()
        
        tsBuffer.clear()
        ledNameBuffer.clear()
        for backline in read_reverse_order(inputFileName):
            analyzeLogLine(backline)

        # Print header
        outfile.write(f"Adjacency chart for MCon logs.  Window size: {associationWindow}ms.\n")
        for i in sorted(hitMatrix):
            outfile.write(",")
            print(i, end="", file=outfile)
        outfile.write("\n")

        # print rows
        for i in sorted(hitMatrix):
            outfile.write(i)
            for j in sorted(hitMatrix):
                if j in hitMatrix[i]:
                    outfile.write(f",{hitMatrix.get(i).get(j)}")
                else:
                    outfile.write(",0")
            outfile.write("\n")
        outfile.close()

if __name__ == '__main__': main()