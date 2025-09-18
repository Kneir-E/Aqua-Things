#ifndef CONFIG_STRING_H
#define CONFIG_STRING_H

#define EXPECTED_VARIABLES 8  // Define how many variables are expected

// Function to remove all whitespaces from the string
String removeWhitespace(String data) {
    String result = "";
    for (int i = 0; i < data.length(); i++) {
        if (!isWhitespace(data.charAt(i))) {
            result += data.charAt(i);
        }
    }
    return result;
}

// Function to split the string by ';' and store the result in an array
int splitString(String data, char delimiter, String resultArray[], int maxTokens) {
    int tokenCount = 0;
    int startIndex = 0;
    int endIndex = 0;

    while ((endIndex = data.indexOf(delimiter, startIndex)) != -1) {
        if (tokenCount < maxTokens) {
            resultArray[tokenCount++] = data.substring(startIndex, endIndex);
            startIndex = endIndex + 1;
        } else {
            break;  // Prevent overflow if too many tokens
        }
    }

    // Add the last token (if any)
    if (startIndex < data.length() && tokenCount < maxTokens) {
        resultArray[tokenCount++] = data.substring(startIndex);
    }

    return tokenCount;  // Return the number of tokens found
}

#endif