# CrazyUncleBurton Markdown Template

by Bryan A. "CrazyUncleBurton" Thompson
Last Updated 3/7/2026

<https://www.markdownguide.org/>

## Paragraphs

To create paragraphs, use a blank line to separate one or more lines of text.

## Line Breaks

To create a line break or new line (<br>), end a line with two or more spaces, and then type return.

## Bold

To bold text, add two asterisks or underscores before and after a word or phrase. To bold the middle of a word for emphasis, add two asterisks without spaces around the letters.

## Italic

To italicize text, add one asterisk or underscore before and after a word or phrase. To italicize the middle of a word for emphasis, add one asterisk without spaces around the letters.

## Bold and Italic

To emphasize text with bold and italics at the same time, add three asterisks or underscores before and after a word or phrase. To bold and italicize the middle of a word for emphasis, add three asterisks without spaces around the letters.

## Blockquotes

To create a blockquote, add a > in front of a paragraph.

## Blockquotes with Multiple Paragraphs

Blockquotes can contain multiple paragraphs. Add a > on the blank lines between the paragraphs.

## Nested Blockquotes

Blockquotes can be nested. Add a >> in front of the paragraph you want to nest.

## Blockquotes with Other Elements

Blockquotes can contain other Markdown formatted elements. Not all elements can be used — you’ll need to experiment to see which ones work.

## Lists

### Ordered Lists

To create an ordered list, add line items with numbers followed by periods. The numbers don’t have to be in numerical order, but the list should start with the number one.

### Ordered List Best Practices

CommonMark and a few other lightweight markup languages let you use a parenthesis ()) as a delimiter (e.g., 1) First item), but not all Markdown applications support this, so it isn’t a great option from a compatibility perspective. For compatibility, use periods only.

### Unordered Lists

To create an unordered list, add dashes (-), asterisks (*), or plus signs (+) in front of line items. Indent one or more items to create a nested list.

### Starting Unordered List Items With Numbers

If you need to start an unordered list item with a number followed by a period, you can use a backslash (\) to escape the period.

### Adding Elements in Lists

To add another element in a list while preserving the continuity of the list, indent the element four spaces or one tab, as shown in the following examples.

## More On Lists

You can nest an unordered list in an ordered list, or vice versa.

1. First item
2. Second item
3. Third item
    - Indented item
    - Indented item
4. Fourth item

## Code

To denote a word or phrase as code, enclose it in backticks (`).

## Code Blocks

Code blocks are normally indented four spaces or one tab. When they’re in a list, indent them eight spaces or two tabs.

## Escaping Backticks

If the word or phrase you want to denote as code includes one or more backticks, you can escape it by enclosing the word or phrase in double backticks (``).

## Code Snippet (with syntax highlighting)
```cpp
pinMode(LED, OUTPUT);
```

## Inline Images

    ![Tux, the Linux mascot](/assets/images/tux.png)

## Horizontal Rules

To create a horizontal rule, use three or more asterisks (***), dashes (---), or underscores (___) on a line by themselves.

## Links

To create a link, enclose the link text in brackets (e.g., [Duck Duck Go]) and then follow it immediately with the URL in parentheses (e.g., (https://duckduckgo.com)).

## Adding Titles to Links

You can optionally add a title for a link. This will appear as a tooltip when the user hovers over the link. To add a title, enclose it in quotation marks after the URL.

## URLs and Email Addresses

To quickly turn a URL or email address into a link, enclose it in angle brackets.

<https://www.markdownguide.org>
<fake@example.com>


## Formatting Links

To emphasize links, add asterisks before and after the brackets and parentheses. To denote links as code, add backticks in the brackets.

I love supporting the **[EFF](https://eff.org)**.
This is the *[Markdown Guide](https://www.markdownguide.org)*.
See the section on [`code`](#code).

## Formatting the First Part of the Link

The first part of a reference-style link is formatted with two sets of brackets. The first set of brackets surrounds the text that should appear linked. The second set of brackets displays a label used to point to the link you’re storing elsewhere in your document.

Although not required, you can include a space between the first and second set of brackets. The label in the second set of brackets is not case sensitive and can include letters, numbers, spaces, or punctuation.

This means the following example formats are roughly equivalent for the first part of the link:

    [hobbit-hole][1]
    [hobbit-hole] [1]

## Formatting the Second Part of the Link

The second part of a reference-style link is formatted with the following attributes:

    The label, in brackets, followed immediately by a colon and at least one space (e.g., [label]: ).
    The URL for the link, which you can optionally enclose in angle brackets.
    The optional title for the link, which you can enclose in double quotes, single quotes, or parentheses.

## Linking Images

To add a link to an image, enclose the Markdown for the image in brackets, and then add the link in parentheses.

[![An old rock in the desert](/assets/images/shiprock.jpg "Shiprock, New Mexico by Beau Rogers")](https://www.flickr.com/photos/beaurogers/31833779864/in/photolist-Qv3rFw-34mt9F-a9Cmfy-5Ha3Zi-9msKdv-o3hgjr-hWpUte-4WMsJ1-KUQ8N-deshUb-vssBD-6CQci6-8AFCiD-zsJWT-nNfsgB-dPDwZJ-bn9JGn-5HtSXY-6CUhAL-a4UTXB-ugPum-KUPSo-fBLNm-6CUmpy-4WMsc9-8a7D3T-83KJev-6CQ2bK-nNusHJ-a78rQH-nw3NvT-7aq2qf-8wwBso-3nNceh-ugSKP-4mh4kh-bbeeqH-a7biME-q3PtTf-brFpgb-cg38zw-bXMZc-nJPELD-f58Lmo-bXMYG-bz8AAi-bxNtNT-bXMYi-bXMY6-bXMYv)

## HTML 

To use HTML, place the tags in the text of your Markdown-formatted file.

This **word** is bold. This <em>word</em> is italic.



