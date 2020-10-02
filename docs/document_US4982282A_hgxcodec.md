# US4982282A

## Image signal compression encoding apparatus and image signal expansion reproducing apparatus

***

## Table of Contents

<details>
<summary>View table of contents</summary>

- [Title - (US4982282A)][title]
- [Heading - (Image signal compression encoding apparatus and image signal expansion reproducing apparatus)][heading]


<!--
[US4982282A][US4982282A]
[Image signal compression encoding apparatus and image signal expansion reproducing apparatus][image-signal-compression-encoding-apparatus-and-image-signal-expansion-reproducing-apparatus]
-->

1. [Table of Contents][table-of-contents]
1. [Abstract][abstract]
2. [Images (8)][images]
3. [Classifications][classifications]
4. [Description][description]
    - [BACKGROUND OF THE INVENTION][background-of-the-invention]
    - [SUMMARY OF THE INVENTION][summary-of-the-invention]
    - [BRIEF DESCRIPTION OF THE DRAWINGS][brief-description-of-the-drawings]
    - [DESCRIPTION OF THE PREFERRED EMBODIMENTS][description-of-the-preferred-embodiments]
5. [Claims (13)][claims]
6. [Patent Citations (4)][patent-citations]
7. [Cited By (39)][cited-by]
8. [Similar Documents][similar-documents]
9. [Priority And Related Applications][priority-and-related-applications]
10. [Priority Applications (2)][priority-applications]
11. [Legal Events][legal-events]
12. [Concepts][concepts]

</details>
<!-- 
1. [Table of Contents][table-of-contents]
1. [Abstract][abstract]
2. [Images (8)][images]
3. [Classifications][classifications]
4. [Description][description]
    - [BACKGROUND OF THE INVENTION][background-of-the-invention]
    - [SUMMARY OF THE INVENTION][summary-of-the-invention]
    - [BRIEF DESCRIPTION OF THE DRAWINGS][brief-description-of-the-drawings]
    - [DESCRIPTION OF THE PREFERRED EMBODIMENTS][description-of-the-preferred-embodiments]
5. [Claims (13)][claims]
6. [Patent Citations (4)][patent-citations]
7. [Cited By (39)][cited-by]
8. [Similar Documents][similar-documents]
9. [Priority And Related Applications][priority-and-related-applications]
10. [Priority Applications (2)][priority-applications]
11. [Legal Events][legal-events]
12. [Concepts][concepts] -->


***

<blockquote>

<h2>US4982282A</h2>

  United States, [**&#128462;** View PDF][pdf-document], [**&#x21b3;** View on Google][google-patent-page]
  <!-- [**&#9776;&#x21b3;** View on Google][google-patent-page] -->

<!-- 
**Info**:
    [Patent citations (4)][patent-citations],
    [Cited by (39)][cited-by],
    [Legal events][legal-events],
    [Similar documents][similar-documents],
    [Priority and Related Applications][priority-and-related-applications]

United States
Download PDF
Find Prior Art
Similar -->

***

**Inventor**: [Osamu Saito][osamu-saito], [Kenji Ito][kenji-ito]

**Current Assignee**: Fujifilm Corp 

***

**Worldwide applications**

**1988** &bull; ~~[JP](https://patents.google.com/patent/JPH0810935B2/en)~~&nbsp; **1989** &bull; ~~[US](https://patents.google.com/patent/US4982282A/en)~~

***

**Application US07/444,282 events**
<details>
<summary><i>Show all events</i></summary>

**1988-12-09** &bull; Priority to JP63-309871

**1988-12-09** &bull; Priority to JP63309871A

**1989-12-01** &bull; Application filed by Fuji Photo Film Co Ltd

**1990-02-01** &bull; Assigned to FUJI PHOTO FILM CO., LTD.

**1991-01-01** &bull; Publication of US4982282A

**1991-01-01** &bull; Application granted

**2007-02-15** &bull; Assigned to FUJIFILM HOLDINGS CORPORATION

**2007-02-26** &bull; Assigned to FUJIFILM CORPORATION

**2009-12-01** &bull; Anticipated expiration

**Status** &bull; Expired - Lifetime

</details>

***

**External links**: [USPTO][external-uspto], [USPTO Assignment][external-uspto-assignment], [Espacenet][external-espacenet], [Global Dossier][external-global-dossier], [Discuss][external-discuss]

</blockquote>

<!--

**1988-12-09** &bull; Priority to JP63-309871

**1989-12-01** &bull; Application filed by Fuji Photo Film Co Ltd

**1991-01-01** &bull; Publication of US4982282A

**1991-01-01** &bull; Application granted

**2009-12-01** &bull; Anticipated expiration

**Status** &bull; Expired - Lifetime

Show all events

**1988-12-09** &bull; Priority to JP63-309871

**1988-12-09** &bull; Priority to JP63309871A

**1989-12-01** &bull; Application filed by Fuji Photo Film Co Ltd

**1990-02-01** &bull; Assigned to FUJI PHOTO FILM CO., LTD.

**1991-01-01** &bull; Publication of US4982282A

**1991-01-01** &bull; Application granted

**2007-02-15** &bull; Assigned to FUJIFILM HOLDINGS CORPORATION

**2007-02-26** &bull; Assigned to FUJIFILM CORPORATION

**2009-12-01** &bull; Anticipated expiration

**Status** &bull; Expired - Lifetime

Hide events

-->



## Image signal compression encoding apparatus and image signal expansion reproducing apparatus


### Abstract

A compression encoder uses an arbitrary normalization coefficient and a preset table to achieve a compression of image signal with a desired compression rate. Since the normalization coefficient and the table data are sent to an image signal decoding and reproducing apparatus together with the compressed image data, the reproducing apparatus can restore the original image from those data items. Furthermore, in the encoder, when an amplitude value of the data exceeds a predetermined value, an overflow sensor means the condition so as to produce normalized data in addition to the Huffman-encoded data. The reproducing apparatus achieves the image signal decoding and reproducing operations by use of the normalized data and the Huffman-encoded data. With these apparatuses, the picture quality can be prevented from being lowered due to an overflow in the encoding operation.


### Images (8)

<!-- [FIX] 1Writer (iOS): img elements starting on new line -->
<style>img {display:inline-block;}</style>

[![][thumb-2]][page-2] [![][thumb-3]][page-3] [![][thumb-4]][page-4] [![][thumb-5]][page-5] [![][thumb-6]][page-6] [![][thumb-7]][page-7] [![][thumb-8]][page-8] [![][thumb-9]][page-9]

#### Rotated Images (3)

[![][thumb-2-rot90]][page-2-rot90] [![][thumb-5-rot90]][page-5-rot90] [![][thumb-6-rot90]][page-6-rot90]

<!-- [![][thumb-2]][page-2] [![][thumb-3]][page-3] [![][thumb-4]][page-4] [![][thumb-5]][page-5] [![][thumb-6]][page-6] [![][thumb-7]][page-7] [![][thumb-8]][page-8] [![][thumb-9]][page-9]

[![][thumb-2-rot90]][page-2-rot90] [![][thumb-3]][page-3] [![][thumb-4]][page-4] [![][thumb-5-rot90]][page-5-rot90] [![][thumb-6-rot90]][page-6-rot90] [![][thumb-7]][page-7] [![][thumb-8]][page-8] [![][thumb-9]][page-9] -->

<!-- [Other page](US4982282A_en%20copy.md) -->

<!-- 
FIG. 1 is a schematic block diagram illustrating an embodiment of an image signal compression encoding apparatus in accordance with the present invention;

FIG. 2 is a block diagram illustrating a non-zero sensor section 18 of FIG. 1;

FIG. 3 is a schematic block diagram illustrating a zero-run counter 20 of FIG. 1;

FIG. 4A is a block diagram illustrating an amplitude sensor section 24 of FIG. 1;

FIG. 4B is a schematic diagram illustrating an overflow sensor circuit 50 of FIG. 4A;

FIG. 5 is a diagram schematically illustrating an additional-bit computing section 26 of FIG. 1;

FIG. 6 is a schematic diagram illustrating a fixed-length item generating buffer 30;

FIG. 7 is a block diagram schematically illustrating an image signal expansion reproducing apparatus for decoding and for reproducing image data compressed and encoded by the compression encoding apparatus of FIG. 1;

FIG. 8 is a diagram illustrating data obtained through a two-dimensional orthogonal transformation;

FIG. 9 is a schematic diagram illustrating encoding operations associated with the run length and a non-zero amplitude;

FIG. 10 is a diagram illustrating an example of weight table data;

FIG. 11 is a schematic diagram illustrating an example of a normalized transformation coefficient;

FIG. 12 is a diagram illustrating relationships between amplitude values of the translation coefficient, amplitude ranges thereof, and additional bits; and

FIG. 13 is a schematic diagram illustrating relationships between amplitude values of the transformation coefficient and amplitude ranges thereof.

[FIG. 1][fig-1] -->

### Classifications

<blockquote>

&#10152;&nbsp;**[H04N19/00](https://patents.google.com/?q=H04N19%2f00)** &nbsp; Methods or arrangements for coding, decoding, compressing or decompressing digital video signals

<details>
<summary><i>View 9 more classifications</i></summary>

&#10152;&nbsp;[H04N19/126](https://patents.google.com/?q=H04N19%2f126) &nbsp; Details of normalisation or weighting functions, e.g. normalisation matrices or variable uniform quantisers

&#10152;&nbsp;[H04N19/136](https://patents.google.com/?q=H04N19%2f136) &nbsp; Incoming video signal characteristics or properties

&#10152;&nbsp;[H04N19/18](https://patents.google.com/?q=H04N19%2f18) &nbsp; Methods or arrangements for coding, decoding, compressing or decompressing digital video signals using adaptive coding characterised by the coding unit, i.e. the structural portion or semantic portion of the video signal being the object or the subject of the adaptive coding the unit being a set of transform coefficients

&#10152;&nbsp;[H04N19/46](https://patents.google.com/?q=H04N19%2f46) &nbsp; Embedding additional information in the video signal during the compression process

&#10152;&nbsp;[H04N19/60](https://patents.google.com/?q=H04N19%2f60) &nbsp; Methods or arrangements for coding, decoding, compressing or decompressing digital video signals using transform coding

&#10152;&nbsp;*[H04N19/124](https://patents.google.com/?q=H04N19%2f124)* &nbsp; Quantisation

&#10152;&nbsp;*[H04N19/13](https://patents.google.com/?q=H04N19%2f13)* &nbsp; Adaptive entropy coding, e.g. adaptive variable length coding [AVLC] or context adaptive * binary arithmetic coding [CABAC]

&#10152;&nbsp;*[H04N19/146](https://patents.google.com/?q=H04N19%2f146)* &nbsp; Data rate or code amount at the encoder output

&#10152;&nbsp;*[H04N19/91](https://patents.google.com/?q=H04N19%2f91)* &nbsp; Entropy coding, e.g. variable length coding [VLC] or arithmetic coding

</details></blockquote>


### Description

#### BACKGROUND OF THE INVENTION

1\. Field of the Invention

The present invention relates to an image signal compression encoding apparatus and to an image signal expansion reproducing apparatus, and in particular, to an image signal compression encoding apparatus in which after a two-dimensional orthogonal transformation of a video signal, the video signal can be normalized by use of a desired normalization coefficient and to an image signal expansion reproducing apparatus for decoding data thus encoded.

2\. Description of the related Art

When loading a memory with digital image data such as image data shot by an electronic still camera, in order to reduce the amount of data to minimize the storage capacity of the memory, various kinds of compression and encoding operations are performed on the image data. Particularly, two-dimensional orthogonal encoding has been commonly employed because the encoding can be accomplished with a high compression rate and image distortion in the encoding can be suppressed.

In such a two-dimensional orthogonal transformation of a video signal, the image data is subdivided into a predetermined number of blocks. The transformation is conducted on image data of each block. Obtained image data, namely, a transformation coefficients are compared with a predetermined threshold value and the that portion does not exceed the threshold value is truncated (i.e. a coefficient truncation is achieved.) As a result, thereafter, any translation coefficients not exceeding the threshold are treated as data of zero in the processing. The transformation coefficients, which have undergone the truncation, are then divided by using a predetermined quantization step value, namely, a normalization coefficient to be quantized or normalized depending on the step width. Through the operation above, the value of the translation coefficients, namely, the dynamic range of the amplitude can be suppressed.

The normalized translation coefficients are thereafter encoded. By the way, the transformation coefficients include data items arranged from a low-frequency range to a high-frequency range depending on the magnitude of each block of the image data. Since the data of the normalized transformation coefficients become 0 in the high-frequency component, run-length encoding is achieved in which the original data are translated into a continuation length of value 0, namely, a so-called run length of 0 and a value of data including values of other than 0, namely, a so-called amplitude of non-zero. The resultant data is then subjected to a two-dimensional Huffman encoding to produce compressed image data thus encoded.

In the two-dimensional orthogonal transformation coding, by altering the value of the normalization coefficient, the image data can be encoded with various compression rates. For example, with a large value of the normalization coefficient, the normalized translation coefficient data takes a small value. Consequently, the compression rate of the image data is increased and the picture quality of the attained data is lowered. Conversely, for a small value of the normalization coefficient, the image data is compressed with a small compression rate and hence a high picture quality is developed for the attained data.

As a consequence, although the normalization is to be achieved with various kinds of normalization coefficients, normalization coefficient data used in an inverse normalization to be conducted by a reproducing apparatus is required to be changed. This leads to a problem that the normalization coefficients cannot be arbitrarily selected. In particular, after the orthogonal translation, when the compression rate of the encoding is desired to be varied between the low-frequency and high-frequency components, there arises a problem of a difficulty in setting the normalization coefficient to the different values.


#### SUMMARY OF THE INVENTION

It is therefore an object of the present invention to provide an image signal compression encoding apparatus wherein normalization of a transformation coefficients after an orthogonal transformation is performed, and the normalization can be achieved by setting various normalization data items which provides an image signal expansion reproducing apparatus capable of appropriately decoding the data produced through the compression encoding, thereby solving the problems of the conventional technology.

In accordance with the present invention, an image signal compression encoding apparatus in which digital image data constituting a screen image is subdivided into a plurality of blocks to achieve a two-dimensional orthogonal transformation encoding on image data of each block includes orthogonal transformation means for conducting a two-dimensional orthogonal transformation of digital image data thus subdivided into plural blocks, normalizing means for normalizing the data transformed by the orthogonal transformation means, table data store means for storing therein table data employed by the normalizing means for the normalization, encode means for encoding the data normalized by the normalizing means, and output data generating means for generating output data comprising the data encoded by the encode means. The output data generating means produces, in addition to the data encoded by the encode means, the table data used by the normalizing means to achieve the normalization.

Furthermore, in accordance with the present invention, an image signal expansion reproducing apparatus for receiving digital image data of a screen which has undergone the compression encoding to achieve a two-dimensional inverse transformation encoding on the data including input means for inputting therefrom data comprising image data, decode means for decoding image data supplied from the input means, inverse normalizing means for inversely normalizing the data decoded by the decode means, table data supply means for selecting table data to be employed by the inverse normalizing means for the inverse normalization so as to supply the data to the inverse normalizing means, and orthogonal inverse transformation means for achieving a two-dimensional orthogonal inverse transformation on the data thus inversely normalized by the inverse normalizing means, the inverse normalizing means achieving the inverse normalization based on table data included in data supplied thereto.


#### BRIEF DESCRIPTION OF THE DRAWINGS

The objects and features of the present invention will become more apparent from the consideration of the following detailed description taken in conjunction with the accompanying drawings in which:

[FIG. 1][fig-1] is a schematic block diagram illustrating an embodiment of an image signal compression encoding apparatus in accordance with the present invention;

[FIG. 2][fig-2] is a block diagram illustrating a non-zero sensor section 18 of [FIG. 1][fig-1];

[FIG. 3][fig-3] is a schematic block diagram illustrating a zero-run counter 20 of [FIG. 1][fig-1];

[FIG. 4A][fig-4a] is a block diagram illustrating an amplitude sensor section 24 of [FIG. 1][fig-1];

[FIG. 4B][fig-4b] is a schematic diagram illustrating an overflow sensor circuit 50 of [FIG. 4A][fig-4a];

[FIG. 5][fig-5] is a diagram schematically illustrating an additional-bit computing section 26 of [FIG. 1][fig-1];

[FIG. 6][fig-6] is a schematic diagram illustrating a fixed-length item generating buffer 30;

[FIG. 7][fig-7] is a block diagram schematically illustrating an image signal expansion reproducing apparatus for decoding and for reproducing image data compressed and encoded by the compression encoding apparatus of [FIG. 1][fig-1];

[FIG. 8][fig-8] is a diagram illustrating data obtained through a two-dimensional orthogonal transformation;

[FIG. 9][fig-9] is a schematic diagram illustrating encoding operations associated with the run length and a non-zero amplitude;

[FIG. 10][fig-10] is a diagram illustrating an example of weight table data;

[FIG. 11][fig-11] is a schematic diagram illustrating an example of a normalized transformation coefficient;

[FIG. 12][fig-12] is a diagram illustrating relationships between amplitude values of the translation coefficient, amplitude ranges thereof, and additional bits; and

[FIG. 13][fig-13] is a schematic diagram illustrating relationships between amplitude values of the transformation coefficient and amplitude ranges thereof.


#### DESCRIPTION OF THE PREFERRED EMBODIMENTS

Referring now to the drawings, a description will be given in detail of an image signal compression encoding apparatus and an image signal expansion reproducing apparatus in accordance with the present invention.

[FIG. 1][fig-1] illustrates an embodiment of an image signal compression encoding apparatus in accordance with the present invention.

The system includes a blocking section 12, which includes a frame buffer to be loaded with a frame of still video data shot by an electronic still camera and supplied thereto via an input terminal 10. The image data of a frame stored in the blocking section 12 is subdivided into a plurality of blocks. The content of each block is read out therefrom so as to be fed to a two-dimensional orthogonal transformer 14. The transformer section 14 achieves a two-dimensional orthogonal transformation on the image data of each block. The translation may be any one of known two-dimensional orthogonal transformations such as a discrete cosine transformation and Hadamard transform coding.

The image data of each block which has undergone the two-dimensional orthogonal translation in the translator 14 is arranged in a two-dimensional array as illustrated in [FIG. 8][fig-8] such that lower-order data is located in the upper-left portion and higher-order data is placed in the lower-right portion as indicated by arrows. The two-dimensional orthogonal transformer 14 produces an output to be fed to a normalizer 16.

The normalizer 16 conducts a coefficient truncation on the image data which has undergone the two-dimensional orthogonal transformation in the transformer 14, namely, on translation coefficients so as to normalize the data thereafter. In the coefficient truncation, the transformation coefficients which have undergone the orthogonal transformation are compared with a predetermined threshold value to truncate a portion of data not exceeding the threshold value. In the normalization, the translation coefficients after the coefficient truncation are divided by a predetermined quantization step value, thereby achieving a quantization of the image data.

Connected to the normalizer 16 is a multiplier 34 for inputting the quantization step value to the normalizer 16. The multiplier 34 is linked to a normalization coefficient storage 22 and a selector 36, which is in turn connected to a weight table storage 38. The normalization coefficient storage 22 and the selector 36 are connected to an operator's console 40 for supplying therefrom the system with an instruction from the operator.

The weight table storage 38 is loaded with data of various kinds of weight tables Ts, as illustrates in [FIG. 10][fig-10], to be employed for the normalization in the normalizer 16. The table data items are delivered to the selector 36. Depending on an instruction of the operator supplied from the operator's console 40, a weight table T is selected by the selector 36 so as to be fed to the multiplier 34. On the other hand, according to an instruction of the operator supplied from the operator's console 40, a normalization coefficient stored in the normalization coefficient storage 22 is selectively obtained therefrom so as to be sent to the multiplier 34. The weight table content is multiplied by the data of the normalization coefficient in the multiplier 34, which thus attains a quantization step value to be adopted in the normalization.

The data of the quantization step value in the multiplier 34 are supplied to the normalizer 16, which divides by the obtained value the transformation coefficient supplied from the two-dimensional transformer 14 has undergone the coefficient truncation, thereby accomplishing the normalization. The multiplier 34 delivers an output of the quantization step value, in addition to the normalizer 16, to a multiplexer 64.

In the translation coefficient, since the low-frequency component is more effective as data when compared with the high-frequency component, the weight table T as illustrated in [FIG. 10][fig-10] is loaded with, for example, smaller values in the low-frequency portion and larger values in the high-frequency portion. The normalization of the data is accomplished by dividing transformation coefficients which have undergone the coefficient truncation by a value αT attained by multiplying the data of the table T by the normalization coefficient α. Assuming the translation coefficients prior to the normalization to be X, the coefficient X' after the normalization is represented as follows.

```X'=X/(αT)```

For example, the table data T is multiplied by α in which the low-frequency and high-frequency components of the transformation coefficient X are respectively associated with the low-frequency and high-frequency components of the table T. The translation coefficients X are divided by the resultant value.

As described above, with the provision of the weight table T, in place of a uniform division of the translation coefficient X by a value α, a smaller value can be adopted as a divider for the division associated with the low-frequency portion to minimize the compression rate, whereas a larger value can be used for the division associated with the low-frequency portion to increase the compression rate. Furthermore, when compressing data in a high picture quality mode, owing to the small value of the normalization coefficient α, by setting a larger value to the low-frequency component and a smaller value to the high-frequency component of the weight table T, an occurrence of an overflow which will concentrate on the low-frequency component can be reduced.

The output from normalizer 16 is an n-bit transformation coefficient normalized as illustrates in [FIG. 11][fig-11]. The coefficient represented with n-bit data takes, as can be seen from this figure, a value ranging from 2<sup>n-1</sup> to -2<sup>n-1</sup>. That is, in this example, a half portion including n-1 bits represents a value of 0 or a value in a range from 2<sup>n-1</sup> to 1 and another half portion including n-1 bits denoting a negative value ranging from -1 to -2<sup>n-1</sup>. Of those values, the negative data items from -1 to -(2<sup>n-1</sup> -1) are 2's complements associated with the positive data items ranging from 1 to 2<sup>n-1</sup> -1, respectively.

For the normalized transformation coefficient, the n-bit data items are arranged in a two-dimensional array in a similar fashion as employed for the data items prior to the normalization as illustrated in [FIG. 8][fig-8]. These data items are sequentially produced through a zigzag scanning as illustrated in [FIG. 9][fig-9].

The normalizer 16 delivers the output to a non-zero sensor 18, an amplitude sensor 24, and an additional-bit computing section 26.

The non-zero sensor 18 is supplied with transformation coefficient data including n bits as illustrates in [FIG. 2][fig-2] to be received by inverters 401, 402, . . . and 40n thereof. These inverters deliver the respective outputs to a NAND circuit 42. Each bit of the n-bit transformation coefficient data is supplied to the associated one of the inverters 401, 402, . . . and 40n as data of "1" or "0". For a data item "1", the inverter produces "0". As a consequence regardless of the outputs from the other inverters, the NAND circuit 42 generates an output "1". This value "1" denotes a non-zero transformation coefficient data, namely, that the n-bit coefficient is other than 0. Conversely, when all bits of the translation coefficient data are "0", each inverter produces an output "1". Consequently, the NAND circuit generates an output "0". This value "0" designates a zero transformation coefficient, namely, that the n-bit coefficient is 0. The non-zero sensor 18 delivers the output to a zero-run counter 20.

The zero-run counter 20 of this embodiment includes an inverter 44 and a six-bit counter 46 as illustrates in [FIG. 3][fig-3]. The inverter 44 is supplied with the output from the non-zero sensor 18, namely, a zero or non-zero signal so as to produce an inverted signal thereof. The output from the inverter 44 is inverted so as to be fed to a clear terminal CLR of the counter 46. On receiving a non-zero signal "1" from the non-zero sensor 18, a signal "0" is supplied to the clear terminal CLR to clear the counter 46. The counter 46 is supplied with a translation coefficient transfer check clock via a clock input terminal CK from the two-dimensional orthogonal transformer 14 so as to count the clocks. When a signal "0" is received by the clear terminal CLR, the count value is cleared. As a consequence, the counter 46 continues the counting while the zero signal "0" being received from the non-zero sensor 18. In response thereto, the length of a zero run is counted in the zigzag scan of [FIG. 9][fig-9].

In this embodiment, the block size is 8×8=64 and the maximum number of continuous zero bits is limited to 64. As a consequence, the zero-run length can be represented with data including six bits. The zero-run counter 20 delivers the output to a two-dimensional Huffman encoder 28.

The amplitude sensor 24 is supplied with a translation coefficient from the normalizer 16 so as to produce an amplitude range bit and an overflow signal, which will be described later. As illustrated in [FIG. 4A][fig-4a], the amplitude sensor 24 includes an absolute value generating circuit 48. The absolute value generator circuit 48 receives the normalized transformation coefficient including n bits from the normalizer 16 so as to create an absolute value thereof. That is, in a case where the inputted n-bit data is zero or a positive value in a range from 2<sup>n-1</sup> to 1 of [FIG. 11][fig-11], the received data is directly outputted from the circuit 48. Whereas for a negative value in a range from -1 to -2<sup>n-1</sup>, a 2' complement is generated so as to invert the sign symbol and producing an absolute value of the input data.

Of the n bits produced from the absolute value generating circuit 48, n-8 high-order bits are sent to an overflow sensor circuit 50. The sensor 50 includes, as illustrates in [FIG. 4B][fig-4b], an OR circuit 54 receiving n-8 inputs. When either one of the n-8 inputs is "1", the circuit 54 produces an overflow signal "1". In this embodiment, as illustrates in [FIG. 12][fig-12], 8-bit data taking a value ranging from -127 to 127 associated with a level of the translation coefficient is compressed and encoded so as to transmit the resultant data. The data beyond the range from -127 to 127 is related to an overflow and hence additional data is added to the resultant data for the transmission. As a consequence, a portion exceeding eight bits, namely, the n-8 high-order bits include "1", an overflow is assumed and hence the overflow sensor 50 produces an overflow signal "1" in this case. The output from the circuit 50 is supplied to a priority encoder 52 and the additional bit computing section 26 of [FIG. 1][fig-1].

The priority encoder 52 is supplied with seven low-order bits of the n bits produced from the absolute value generating circuit 48. Since the transformation coefficient is data representing an absolute value created by the circuit 48, the data is in a range from -127 to 127 and hence can be represented with seven bits of the original eight bits where a bit is removed for use as a sign identification. In this situation, seven low-order bits of n bits from the circuit 48 are adopted to represent data in the range. As illustrated in [FIG. 13][fig-13], the priority encoder 52 is responsive to an output OF from the overflow sensor 50 and to the seven low-order bits from the absolute value generating circuit 48 so as to produce three-bit data indicating an amplitude range.

In a case, as illustrated in [FIG. 13][fig-13], where the overflow sensor 50 produces a signal "0", the transformation coefficient is expressed with seven low-order bits. Namely, depending on the seven low-order bits, the three-bit data is generated to indicate the amplitude range. Data of the amplitude range is set as illustrated in [FIG. 12][fig-12] according to the range of the data of seven low-order bits included in the translation coefficient. In [FIG. 13][fig-13], positions denoted by x in the seven low-order bits may take either one of the values "1" and "0".

For an overflow signal "1", there exists data exceeding eight low-order bits and hence "000" is produced for the amplitude range, namely, a signal indicating an overflow is generated as shown in [FIG. 13][fig-13].

In this embodiment, a three-bit amplitude range is developed in association with a range of the amplitude value as illustrated in [FIG. 12][fig-12]. For example, for an amplitude value -1 or 1, the amplitude range is determined as "001". In a case where the amplitude is in a range from -127 to -64 or from 64 to 127, the amplitude range is attained as "111". Incidentally, the additional bit of [FIG. 13][fig-13] designates the number of bits necessary for identifying a value of data for which the amplitude range, namely, the range of the amplitude value is specified by the three-bit amplitude range data.

For example, for the amplitude range data "010" of [FIG. 12][fig-12], available amplitude values include -3, -2, 2, and 3. In this situation, two bits are required to specify either one of these four data items. Consequently, three-bit data is necessary to represent data ranging from -3 to 3. However, since the amplitude range data specifies the amplitude range, only two bits are required for the additional-bit data to identify the data in the range. As described above, when n+1 bits are necessary to denote the amplitude data itself, if the amplitude range is specified by the amplitude range data, the additional-bit data needs to only include n bits.

The amplitude range data produced from the priority encoder 52 is fed to the two-dimensional Huffman encoder 28 and a fixed-length item generating buffer 30.

The two-dimensional Huffman encoder 28 receives data of a zero-run length from the zero-run counter 20 and data of an amplitude range from the amplitude sensor 24 to achieve a two-dimensional Huffman encoding thereon. In this embodiment, the zero-run length and the amplitude range are respectively expressed with six and three bits, namely, nine-bit data including a combination of these data items is subjected to the Huffman encoding. The two-dimensional Huffman encoder 28 supplied the fixed-length item generating buffer 30 with data of a predetermined number of bits (m bits) thus encoded and a length of the encoded data or a so-called Huffman code length.

The additional-bit computing section 26 is includes a subtractor 56 and a selector 58 as illustrated in [FIG. 5][fig-5]. This section 26 receives a normalized transformation coefficient from the normalizer 16 and an overflow signal from the amplitude sensor 24. The subtractor 56 is supplied with data of seven low-order bits of the translation coefficient and the most-significant bit, MSB, as the highest-order bit. The data of MSB signal is subtracted from the data of seven low-order bits. The MSB signal is "0" or "1" when the data of the seven low-order bits is positive or negative, respectively. The subtractor 56 sends a subtraction result represented with data including at most seven bits to an input A of the selector 58.

The selector 58 has another input B, which directly receives the n-bit transformation coefficient from the normalizer 16. Furthermore, the selector 58 receives an overflow signal from the amplitude sensor 24. For an overflow signal "0" indicating the absence of an overflow, the selector 58 selects data including at most seven bits received from the input A. For an overflow signal "1" denoting the presence of an overflow, the selector 58 selects the n-bit translation coefficient data attained from the input B. As a consequence, the data outputted from the additional-bit computing section 26 includes at most seven bits or the n-bit data depending on the absence or presence of an overflow, respectively.

The fixed-length item generating buffer 30 includes, as illustrated in [FIG. 6][fig-6], a bit length computing section 60 and a buffer 62. The bit length computing section 60 is supplied with the amplitude range data from the amplitude sensor 24 and the Huffman code length from the two-dimensional encoder 28. Based on the amplitude range and the Huffman code length, the computing section 60 determines the total number of bits included in data obtained through the Huffman encoding and additional-bit data to be added thereto so as to supply the buffer 62 with an address signal for a write operation of the data associated with the number of bits. Since the additional-bit data includes at most seven bits when an overflow does not occur and n bits at an occurrence of the overflow, a signal including the additional-bit data and the bits of data obtained through the Huffman encoding is delivered to the buffer 62.

The buffer 62 further receives additional bits ranging from one bit to n bits from the additional-bit computing section 26 and the Huffman-encoded data from the two-dimensional Huffman encoder 28. Depending on the total data length of the encoded data and the additional-bit data, the bit length computing section 60 produces an address signal. These data items are written in the buffer 62 at an address designated by the address signal.

The data stored in the buffer 62 is transmitted to the multiplexer 64 of [FIG. 1][fig-1] after the Huffman-encoded data is combined with the additional-bit data so as to be sent thereto in the unit of data including a predetermined number of bits. The multiplexer sequentially selects the encoded data sent from the fixed-length item generating buffer 30 or the quantization step value supplied from the multiplier 34. The selected data or value is sent from an output terminal 32 to a transmission route or is written on a recording medium such as a magnetic disk.

According to a compression encoder as described above, the input image data is subdivided into a plurality of blocks by the blocking section 12 for undergoing the two-dimensional orthogonal translation by the two-dimensional orthogonal transformer 14. The transformation coefficient attained through the orthogonal translation is subjected to the coefficient truncation and the normalization as described above. The resultant data is sent to the non-zero sensor 18 and the amplitude sensor 24 sense the non-zero and the amplitude, respectively. Thereafter, the zero-run length attained by the zero-run counter 20 and the amplitude range data from the amplitude sensor 24 are subjected to the two-dimensional Huffman encoding in the two-dimensional Huffman encoder 28.

The encoded data is checked in the fixed-length item generating buffer 30 to determine whether or not an overflow occurs in consideration of the amplitude range data. If an overflow does not occur, the additional bit data ranging from one bit to seven bits is produced for the encoded data. Otherwise, the additional bit data including n bits is added to the encoded data.

Consequently, when the overflow does not occur, the which data has undergone the two-dimensional Huffman encoding and the additional-bit data including the fewer number of bits is transmitted or recorded so as to achieve the compression encoding of image data. On the other hand, at an occurrence of an overflow, the produced data includes the encoded data and the additional-bit data. As a consequence, the additional bit represents the image data itself. As a result, in a reproduction of the image data, the picture quality is prevented from being deteriorated due to the overflow.

In the conventional compression encoder, when the amplitude of the normalized translation coefficient is beyond the predetermined range, namely, when the number of bits of the transformation coefficient exceeds the number of the present bits, a data overflow takes place. For example, in the n-bit data as illustrated in [FIG. 11][fig-11], if the amplitude range is specified as n-1 bits, when the n-bit data takes a negative value, the data "1" of the upper-most bit is ignored so as to set the amplitude range based on the other n-1 bits. Since the produced data includes the additional bit which data has and the data undergone the Huffman encoding based on the amplitude range and the zero-run length, when decoding the data, it is impossible to decode the data "1" of the upper-most bit. Namely, the same decoding result is attained from the data items respectively having "1" and "0" at the upper-most position. As a result, an image displayed with the decoded image data is attended with an inversion in white and black portions and hence the picture quality is deteriorated.

Particularly, in order to reduce the compression rate, when the value of the normalization coefficient employed by the normalizer 16 for the normalization is reduced, the normalized transformation coefficients takes a large value. As a consequence, an overflow is likely to occur and hence the picture quality is lowered. Namely, this leads to a disadvantage that even if the compression rate is minimized to attain a high picture quality, it is impossible to develop the desired picture quality.

In contrast thereto, according to the apparatus of the embodiment, when an overflow occurs based on the preset amplitude range, the system produces output data by adding the additional-bit data representing the image data to the encoded data which has undergone the compression coding. As a consequence, the value of the image data is denoted by the additional-bit data. By decoding the additional-bit data, the original image data can be reproduced and hence no deterioration of the picture quality due to the overflow occurs.

Furthermore, the amplitude range data specifies the amplitude range, namely, a range of the amplitude value. The range data is Huffman-encoded together with the zero-run length. Consequently, when data causes an overflow as described above, since the data represents a value obtained by removing the value in the range specified by the amplitude range data, the value of additional bits to identify the data can be expressed with bits of which the number is less by one than the number required to represent the data itself.

For example, as illustrated in [FIG. 12][fig-12], when the amplitude value data is in a range from -255 to -128 and in a range from 128 to 255, the amplitude value data can be identified by additional-bit data including eight bits. In a case where the amplitude value data is in a range from -255 to 255, although nine-bit data is naturally necessary to express this data, since the data in the range from -127 to 127 is beforehand removed, the data can be identified by eight bits.

In addition, with the provision of the weight table T, in place of the operation to uniformly divide the translation coefficient by a value α, various values can be used for the normalization depending on the components of the translation coefficient. For example, for the division, smaller and larger values may be employed for the low-frequency and high-frequency components, respectively. In this case, for the compression encoding operation, a smaller compression rate can be applied to the low-frequency components which are more effective as data, whereas a larger compression rate can be used for the high-frequency components which are less effective as data. As a result, the compression rate as well as the picture quality can be increased. Conversely, when a large compression rate is adopted for the low-frequency component in the compression encoding, the overflow can be prevented.

The weight table T may be loaded with arbitrary predetermined data. Moreover, data inputted from the operator's console 40 to specify a weight table T and data supplied from the console 40 to specify a normalization coefficient or the table data itself may be delivered to the multiplexer 64 such that an encoding is achieved by use of these data items in a decoder, which will be described later.

[FIG. 7][fig-7] illustrates an embodiment of an image signal expansion reproducing apparatus in accordance with the present invention. This apparatus is employed to achieve an expansion playback operation of image data encoded by the encoder of [FIG. 1][fig-1].

The configuration of [FIG. 7][fig-7] includes a Huffman decoder 72, which has an input terminal 70 to receive image data generated through the compression encoding in the compression encoder of [FIG. 1][fig-1]. Namely, the input terminal 70 receives data obtained through the Huffman encoding achieved by the two-dimensional Huffman encoder 28 of the system illustrated in [FIG. 1][fig-1] and the additional-bit data generated by the additional-bit computing section 26 of the system. Of these data items, the Huffman-encoded data is fed to the Huffman decoder 72. The Huffman decoder 72 decodes the received data based on data transmitted from a Huffman table, not shown, so as to obtain data of the zero-run length and data of the non-zero amplitude range. The zero-run data and the non-zero amplitude range data created from the Huffman decoder 72 are delivered to a zero-run generator 74 and a transformation coefficient decoder 76, respectively. Moreover, the Huffman decoder 72 outputs a zero/non-zero switch signal to a multiplexer 78.

On the other hand, the additional-bit data from the input terminal 70 is supplied to the transformation coefficient decoder 76.

The zero-run generator 74 produces, based on the zero-run length data from the Huffman decoder 72, data of zeros of which the number is associated with the run length. That is, the number of zeros is equivalent to the run length. The zero-run data created from the zero-run generator 74 is transmitted to the multiplexer 78.

The transformation coefficient decoder 76 decodes an n-bit non-zero translation coefficient based on the non-zero amplitude range data from the Huffman decoder 72 and the additional-bit data from the input terminal 70. If the amplitude range data is other than "000", it is indicated that the overflow sensor 50 of the apparatus of [FIG. 1][fig-1] has not sensed an overflow. Consequently, the translation coefficient decoder 76 accomplishes the decoding by use of the three-bit amplitude range data and the additional-bit data to compute the non-zero translation coefficient. If the amplitude range data is "000", the overflow sensor 50 of the apparatus of [FIG. 1][fig-1] is assumed to have sensed an overflow. Consequently, the transformation coefficient decoder 76 achieves the decoding by use of the additional-bit data to attain the non-zero transformation coefficient. In a case of an occurrence of an overflow, since the non-zero transformation coefficient itself has been transmitted as the additional-bit data, this data is directly adopted as an output from the decoded transformation coefficient. The transformation coefficient decoder 76 delivers the output to the multiplexer 78.

The multiplexer 78 selects, depending on the zero/non-zero switch signal from the Huffman decoder 72, the zero-run data from the zero-run generator 74 or the n-bit non-zero transformation coefficient from the transformation coefficient decoder 76. When the zero/non-zero switch signal indicates zero, the zero-run data from the zero-run generator 74 is selected. Whereas, when the non-zero is indicated, the n-bit non-zero transformation coefficient from the transformation coefficient decoder 76 is used. The multiplexer 78 delivers the output to an inverse normalizer 80.

The inverse normalizer 80 is supplied with, in additional to image data from the input terminal 70, data of a product αT between the normalization coefficient and table data. Based on the data αT. the inverse normalizer 80 inversely normalizes the decoded data from the multiplexer 78. That is, the data αT is multiplied by the decoded image data from the multiplexer 78 so as to achieve the inverse normalization. The decoded image data from the multiplexer 78 includes encoded data obtained through a normalization by use of the weight table T as illustrated in [FIG. 10][fig-10]. As a consequence, data as a result of a multiplication between the normalization coefficient α and the weight table data T is supplied to the input terminal to conduct the inverse normalization. By multiplying the data from the multiplexer 78 by the data αT, the inverse normalization is accomplished.

The inverse normalizer 80 sends the output to a two-dimensional inverse transformer 82, which conducts a two-dimensional orthogonal inverse transformation on the inversely normalized data from the inverse normalizer 80. The two-dimensional inverse transformer 82 transmits the output to a block composer 84, which combines a plurality of blocks into image data of an overall screen. The block composer 84 delivers the output to a cathode-ray tube, CRT 86 to reproduce the image thereon. By the way, in lace of the CRT 86, there may be disposed, for example, a printer to which the output is supplied so as to produce a print.

In accordance with the image signal reproducing apparatus above, the image data which has undergone the compression encoding in the encoder of [FIG. 1][fig-1] can be reproduced through the expansion.

In accordance with this apparatus, since the inverse normalizer 80 achieves the inverse normalization based on the image data from the input terminal 70 and the data of the product between the normalization coefficient α and the weight table T, the decoding can be carried out in association with the normalization coefficient and the weight table employed in the encoding. As a consequence, encoded data items obtained through various encoding operations depending on image data items can be appropriately decoded, for thereby reproducing the image data.

As described above, when data encoded by use of a weight table is to be decoded, it is possible in the decoding operation, for example, to adopt a small value and a large value as the divider in the normalization of the low-frequency and high-frequency components, respectively. As a consequence, since the decoding is achieved on encoded data through an encoding such that a smaller compression rate is applied to the low-frequency component which is more significant as data and that a larger compression rate is used for the high frequency component which is less effective as data, the reproduced image develops a high picture quality. Furthermore, the low-frequency component is encoded with a large compression rate so as to prevent an occurrence of the overflow. The obtained data is decoded to reproduce an image with a high picture quality.

According to the image signal reproducing apparatus above, since the decoding can be achieved depending on the received weight table data, and also in a case where various weight tables are used in the image data encoding, the respective image data can be appropriately decoded.

Furthermore, in this reproducing apparatus, as described above, the encoded data from the input terminal 70 is decoded by the Huffman decoder 72 to obtain the zero-run length and the non-zero amplitude range. Thereafter, the zero-run generator 74 and the transformation coefficient decoder 76 produce data of zeros of which the number is associated with the run length and the non-zero translation coefficient data. When the amplitude range is within a predetermined range, the transformation coefficient decoder 76 decodes the transformation coefficient by use of the amplitude range data and the additional-bit data. When the amplitude range is beyond the predetermined range, the transformation coefficient is produced by use of the additional-bit data including n bits. As a consequence, also in a case where the amplitude range is beyond the predetermined range, the translation coefficient can be generated from the additional-bit data. With this provision, it is possible to prevent the occurrence of missing data taking place due to a data overflow in the conventional system in which the decoding is conducted only by use of the amplitude range data. As a result, the picture quality deterioration caused by the data overflow does not appear.

As above, in accordance with the reproducing apparatus, data of the zero-run length and data of the non-zero amplitude range, which each undergone the Huffman encoding, and the additional bit data at an occurrence of an overflow are decoded so as to reproduce an image. As a consequence, image data which has undergone the compression encoding based on an amplitude range can be decoded for the playback of the data. Moreover, also when an overflow occurs with respect to the amplitude range in the compression encoding, the image can be reproduced by use of the additional-bit data.

In accordance with the present invention, since the compression encoder employs table data in the normalization after the orthogonal transformation, various kinds of normalization can be achieved by setting associated table data. Consequently, the compression rate can be arbitrarily set. Furthermore, the overflow in the encoding of image data can be prevented.

In addition, since the table data adopted in the normalization is sent from the compression encoder to the reproducing apparatus, the table data can be used for the inverse normalization in the reproducing apparatus so as to decode image data depending on the data employed in the encoding operation.

While the present invention has been described with reference to the particular illustrative embodiment, it is not to be restricted by those embodiments but only by the appended claims. It is to be appreciated that those skilled in the art can change or modify the embodiment without departing from the scope and spirit of the present invention.



### Claims (13)

What is claimed is:

<p>1. An image signal compression encoding apparatus in which digital image data for a screen is subdivided into a plurality of blocks so as to achieve a two-dimensional orthogonal transformation encoding on image data of each block comprising;
</p><dl><dd><p>orthogonal transforming means for conducting a two-dimensional orthogonal transformation of the digital image data thus subdivided into the plurality of blocks;
    </p><p>normalizing means for normalizing the data transformed by said orthogonal means;
    </p><p>table data store means for storing therein table data employed by said normalizing means for the normalization;
    </p><p>encoder means for encoding the data normalized by said normalizing means; and
    </p><p>output data generating means for generating output data including the data encoded by said encoder means,
    </p><p>said output data generating means producing, in addition to the data encoded by said encoder means, the table data used by said normalizing means to achieve the normalization.
    </p><dl><dd><p>2. An apparatus in accordance with claim 1, wherein said normalizing means uses, in addition to the table data, a normalization coefficient so as to divide the data which has undergone the orthogonal transformation by data obtained from multiplying the normalization coefficient by the table data, thereby achieving the normalization.
        </p><p>3. An apparatus in accordance with claim 1 further comprising;
        </p><dl><dd><p>overflow sense means for sensing a condition that an amplitude value of the normalized data is beyond a predetermined range,
            </p><p>said output data generating means producing in the condition where the amplitude value of the normalized data is beyond the predetermined range and the condition is sensed by said overflow sense means, data identifying the normalized data in addition to data encoded by said encoder means.
        </p></dd></dl><p>4. An apparatus in accordance with claim 3, wherein;
        </p><dl><dd><p>when said overflow sense means fails to sense the condition that the amplitude value of the normalized data is beyond the predetermined range, said output data generating means produces, in addition to the data encoded by said encoder means, additional-bit data including at most seven bits; and
            </p><p>when said overflow sense means senses the condition that the amplitude value of the normalized data is beyond the predetermined range, said output data generating means produces, in addition to the data encoded by said encoder means, data identifying the normalized data, said identifying data including bits of which a number less than a number of bits of the normalized data by one.
        </p></dd></dl><p>5. An apparatus in accordance with claim 3, wherein;
        </p><dl><dd><p>said overflow sense means includes an absolute value generating circuit for computing an absolute value of transformation coefficients supplied thereto and an OR circuit for receiving a predetermined number of high-order bits from said absolute value generating circuit such that the condition that the amplitude value of the normalized data being beyond the predetermined range is sensed by use of an output produced from said OR circuit.
</p></dd></dl></dd></dl></dd></dl><p>6. An image signal exapnsion reproducing apparatus in which digital image data of a screen which has undergone the compression encoding is received so as to achieve a two-dimensional inverse transformation decoding on the received data comprising;
</p><dl><dd><p>input means for inputting therefrom data including image data;
    </p><p>decode means for decoding image data supplied from said input means;
    </p><p>inverse normalizing means for inversely normalizing the data decoded by said decode means;
    </p><p>table data supply means for selecting table data to be employed by said inverse normalizing means for the inverse normalization so as to supply the data to said inverse normalizing means; and
    </p><p>orthogonal inverse transformation means for achieving a two-dimensional orthogonal inverse transformation on the data thus inversely normalized by said inverse normalizing means,
    </p><p>said inverse normalizing means achieving the inverse normalization based on table data included in data supplied thereto.
    </p><dl><dd><p>7. An apparatus in accordance with claim 6, wherein said inverse normalizing means uses, in addition to the table data, an inverse normalization coefficient for multiplying the data decoded by said decode means by data obtained by multiplying the inverse normalization coefficient by the table data, for thereby achieving the inverse normalization.
        </p><p>8. An apparatus in accordance with claim 6 further comprising;
        </p><dl><dd><p>data select means for selecting at least one of the image data decoded by said decode means and the image data supplied from said input means; and
            </p><p>overflow sense means for sensing a condition that an amplitude value of the input data is beyond a predetermined range based on the data decoded by said decode means,
            </p><p>said data select means selecting, when said overflow sense means senses the condition that the amplitude value of the input data is beyond the predetermined range, the input image data in addition to the image data decoded by said decode means.
</p></dd></dl></dd></dl></dd></dl><p>9. A method for achieving a two-dimensional orthogonal transformation encoding on digital image data for a screen subdivided into a plurality of blocks, comprising the steps of:
</p><dl><dd><p>(a) conducting a two-dimensional orthogonal transformation of the digital image data thus subdivided into the plurality of blocks;
    </p><p>(b) normalizing the data transformed at said step (a);
    </p><p>(c) storing table data employed at said step (b);
    </p><p>(d) encoding the data normalized at said step (b); and
    </p><p>(e) generating output data including the data encoded at said step (d) and the table data used at said step (b).
    </p><dl><dd><p>10. A method in accordance with claim 9, wherein said step (b) divides the data transformed at said step (a) by multiplying a normalization coefficient with the table data for achieving the normalization.
        </p><p>11. A method in accordance with claim 9, further comprising the step of:
        </p><dl><dd><p>(f) sensing a condition that an amplitude value of the normalized data is beyond a predetermined range and generating data identifying the normalized data at said step (e) when the amplitude value of the normalized data is beyond the predetermined range.
        </p></dd></dl><p>12. A method in accordance with claim 11, wherein additional bit data including at most seven bits are produced at said step (e) when said step (f) fails to sense that the condition of the normalized data is beyond the predetermined range and data identifying the normalized data when said step (f) senses the condition of the amplitude value of the normalized data being beyond the predetermined range, said identifying data including bits of a number less than a number of bits of the normalized data by one.
        </p><p>13. A method in accordance with claim 11, wherein said step (f) computes an absolute value of transformation coefficients supplied thereto and receives a predetermined number of high-order bits such that the condition that the amplitude value of the normalized data being beyond the predetermined range is sensed.
        </p></dd></dl></dd></dl>

<!-- 
What is claimed is:

1. An image signal compression encoding apparatus in which digital image data for a screen is subdivided into a plurality of blocks so as to achieve a two-dimensional orthogonal transformation encoding on image data of each block comprising;

    * orthogonal transforming means for conducting a two-dimensional orthogonal transformation of the digital image data thus subdivided into the plurality of blocks;
    * normalizing means for normalizing the data transformed by said orthogonal means;
    * table data store means for storing therein table data employed by said normalizing means for the normalization;
    * encoder means for encoding the data normalized by said normalizing means; and
    * output data generating means for generating output data including the data encoded by said encoder means,
    * said output data generating means producing, in addition to the data encoded by said encoder means, the table data used by said normalizing means to achieve the normalization.

        2. An apparatus in accordance with claim 1, wherein said normalizing means uses, in addition to the table data, a normalization coefficient so as to divide the data which has undergone the orthogonal transformation by data obtained from multiplying the normalization coefficient by the table data, thereby achieving the normalization.

        3. An apparatus in accordance with claim 1 further comprising;

            * overflow sense means for sensing a condition that an amplitude value of the normalized data is beyond a predetermined range,
            * said output data generating means producing in the condition where the amplitude value of the normalized data is beyond the predetermined range and the condition is sensed by said overflow sense means, data identifying the normalized data in addition to data encoded by said encoder means.

        4. An apparatus in accordance with claim 3, wherein;

            * when said overflow sense means fails to sense the condition that the amplitude value of the normalized data is beyond the predetermined range, said output data generating means produces, in addition to the data encoded by said encoder means, additional-bit data including at most seven bits; and
            * when said overflow sense means senses the condition that the amplitude value of the normalized data is beyond the predetermined range, said output data generating means produces, in addition to the data encoded by said encoder means, data identifying the normalized data, said identifying data including bits of which a number less than a number of bits of the normalized data by one.

        5. An apparatus in accordance with claim 3, wherein;

            * said overflow sense means includes an absolute value generating circuit for computing an absolute value of transformation coefficients supplied thereto and an OR circuit for receiving a predetermined number of high-order bits from said absolute value generating circuit such that the condition that the amplitude value of the normalized data being beyond the predetermined range is sensed by use of an output produced from said OR circuit.

6. An image signal exapnsion reproducing apparatus in which digital image data of a screen which has undergone the compression encoding is received so as to achieve a two-dimensional inverse transformation decoding on the received data comprising;

    * input means for inputting therefrom data including image data;
    * decode means for decoding image data supplied from said input means;
    * inverse normalizing means for inversely normalizing the data decoded by said decode means;
    * table data supply means for selecting table data to be employed by said inverse normalizing means for the inverse normalization so as to supply the data to said inverse normalizing means; and
    * orthogonal inverse transformation means for achieving a two-dimensional orthogonal inverse transformation on the data thus inversely normalized by said inverse normalizing means,
    * said inverse normalizing means achieving the inverse normalization based on table data included in data supplied thereto.

        7. An apparatus in accordance with claim 6, wherein said inverse normalizing means uses, in addition to the table data, an inverse normalization coefficient for multiplying the data decoded by said decode means by data obtained by multiplying the inverse normalization coefficient by the table data, for thereby achieving the inverse normalization.

        8. An apparatus in accordance with claim 6 further comprising;
            * data select means for selecting at least one of the image data decoded by said decode means and the image data supplied from said input means; and
            * overflow sense means for sensing a condition that an amplitude value of the input data is beyond a predetermined range based on the data decoded by said decode means,
            * said data select means selecting, when said overflow sense means senses the condition that the amplitude value of the input data is beyond the predetermined range, the input image data in addition to the image data decoded by said decode means.

9. A method for achieving a two-dimensional orthogonal transformation encoding on digital image data for a screen subdivided into a plurality of blocks, comprising the steps of:
    
    * (a) conducting a two-dimensional orthogonal transformation of the digital image data thus subdivided into the plurality of blocks;
    * (b) normalizing the data transformed at said step (a);
    * (c) storing table data employed at said step (b);
    * (d) encoding the data normalized at said step (b); and
    * (e) generating output data including the data encoded at said step (d) and the table data used at said step (b).

        10. A method in accordance with claim 9, wherein said step (b) divides the data transformed at said step (a) by multiplying a normalization coefficient with the table data for achieving the normalization.

        11. A method in accordance with claim 9, further comprising the step of:
            * (f) sensing a condition that an amplitude value of the normalized data is beyond a predetermined range and generating data identifying the normalized data at said step (e) when the amplitude value of the normalized data is beyond the predetermined range.
        
        12. A method in accordance with claim 11, wherein additional bit data including at most seven bits are produced at said step (e) when said step (f) fails to sense that the condition of the normalized data is beyond the predetermined range and data identifying the normalized data when said step (f) senses the condition of the amplitude value of the normalized data being beyond the predetermined range, said identifying data including bits of a number less than a number of bits of the normalized data by one.

        13. A method in accordance with claim 11, wherein said step (f) computes an absolute value of transformation coefficients supplied thereto and receives a predetermined number of high-order bits such that the condition that the amplitude value of the normalized data being beyond the predetermined range is sensed.
 -->



### Patent Citations (4)

|Publication number|Priority date|Publication date|Assignee|Title|
|:-----------------|:------------|:---------------|:-------|:----|
|[US4302775A](https://patents.google.com/patent/US4302775A/en)&nbsp;\*|1978-12-15|1981-11-24|Compression Labs, Inc.|Digital video compression system and methods utilizing scene adaptive coding with rate buffer feedback|
|[US4780761A](https://patents.google.com/patent/US4780761A/en)&nbsp;\*|1987-06-02|1988-10-25|Eastman Kodak Company|Digital image compression and transmission system visually weighted transform coefficients
|*Family To Family Citations*| | | | |
|[JPS62150473A](https://patents.google.com/patent/JPS62150473A/en)&nbsp;\*|1985-12-24|1987-07-04|Konishiroku Photo Ind Co Ltd|Radiant ray picture data compressing device|
|[JP2811175B2](https://patents.google.com/patent/JP2811175B2/en)&nbsp;\*|1986-01-27|1998-10-15|富士写真フイルム株式会社|Orthogonal transform coding method for image data|

<small>\* Cited by examiner, † Cited by third party</small>


### Cited By (39)

|Publication number|Priority date|Publication date|Assignee|Title|
|:-----------------|:------------|:---------------|:-------|:----|
|[US5045925A](https://patents.google.com/patent/US5045925A/en)&nbsp;\*|1989-02-16|1991-09-03|Fuji Photo Film Co., Ltd.|Apparatus for achieving compression encoding on components of image signal|
|[US5073820A](https://patents.google.com/patent/US5073820A/en)&nbsp;\*|1989-10-31|1991-12-17|Olympus Optical Co., Ltd.|Image data coding apparatus and coding method|
|[US5101280A](https://patents.google.com/patent/US5101280A/en)&nbsp;\*|1989-05-19|1992-03-31|Fuji Photo Film Co., Ltd.|Device for coding a picture signal by compression|
|[EP0493130A2](https://patents.google.com/patent/EP0493130A2/en)&nbsp;\*|1990-12-28|1992-07-01|Canon Kabushiki Kaisha|Image encoding apparatus optimizing the amount of generated code|
|[FR2671208A1](https://patents.google.com/patent/FR2671208A1/en)&nbsp;\*|1991-01-02|1992-07-03|Ricoh Kk|Quantification process for use in image compression.|
|[US5155592A](https://patents.google.com/patent/US5155592A/en)&nbsp;\*|1989-09-29|1992-10-13|Alcatel N.V.|Image processing system|
|[EP0514188A2](https://patents.google.com/patent/EP0514188A2/en)&nbsp;\*|1991-05-17|1992-11-19|Canon Kabushiki Kaisha|Encoding/decoding apparatus|
|[US5170264A](https://patents.google.com/patent/US5170264A/en)&nbsp;\*|1988-12-10|1992-12-08|Fuji Photo Film Co., Ltd.|Compression coding device and expansion decoding device for a picture signal|
|[US5184229A](https://patents.google.com/patent/US5184229A/en)&nbsp;\*|1988-12-09|1993-02-02|Fuji Photo Film Co., Ltd.|Compression coding device and expansion decoding device for picture signal|
|[GB2258781A](https://patents.google.com/patent/GB2258781A/en)&nbsp;\*|1991-08-13|1993-02-17|Sony Broadcast &amp; Communication|Data compression|
|[EP0539155A2](https://patents.google.com/patent/EP0539155A2/en)&nbsp;\*|1991-10-21|1993-04-28|Canon Kabushiki Kaisha|Image transmitting method|
|[US5216712A](https://patents.google.com/patent/US5216712A/en)&nbsp;\*|1990-12-27|1993-06-01|Kabushiki Kaisha Toshiba|Recording apparatus|
|[US5216516A](https://patents.google.com/patent/US5216516A/en)&nbsp;\*|1990-04-27|1993-06-01|Ricoh Company, Inc.|Orthogonal transformation arithmetic unit|
|[US5216518A](https://patents.google.com/patent/US5216518A/en)&nbsp;\*|1990-09-04|1993-06-01|Canon Kabushiki Kaisha|Image processing method and apparatus for encoding variable-length data|
|[US5218622A](https://patents.google.com/patent/US5218622A/en)&nbsp;\*|1989-12-29|1993-06-08|U.S. Philips Corporation|System of encoding digital signals intended for transmission and/or storage and a corresponding decoding system|
|[WO1993012613A1](https://patents.google.com/patent/WO1993012613A1/en)&nbsp;\*|1991-12-13|1993-06-24|Avid Technology, Inc.|Quantization table adjustment|
|[US5289190A](https://patents.google.com/patent/US5289190A/en)&nbsp;\*|1990-12-27|1994-02-22|Kabushiki Kaisha Toshiba|Recording/reproducing apparatus including control signal indicating high-efficiency coding|
|[US5295077A](https://patents.google.com/patent/US5295077A/en)&nbsp;\*|1991-01-23|1994-03-15|Ricoh Company, Ltd.|Digital electronic still camera|
|[EP0593159A2](https://patents.google.com/patent/EP0593159A2/en)&nbsp;\*|1992-10-09|1994-04-20|Hudson Soft Co., Ltd.|Image processing apparatus|
|[US5317428A](https://patents.google.com/patent/US5317428A/en)&nbsp;\*|1989-04-26|1994-05-31|Canon Kabushiki Kaisha|Image encoding method and apparatus providing variable length bit stream signals|
|[US5329375A](https://patents.google.com/patent/US5329375A/en)&nbsp;\*|1991-02-26|1994-07-12|Matsushita Electric Industrial Co., Ltd.|Information recording/reproducing apparatus|
|[US5335016A](https://patents.google.com/patent/US5335016A/en)&nbsp;\*|1991-01-29|1994-08-02|Olympus Optical Co., Ltd.|Image data compressing/coding apparatus|
|[US5341318A](https://patents.google.com/patent/US5341318A/en)&nbsp;\*|1990-03-14|1994-08-23|C-Cube Microsystems, Inc.|System for compression and decompression of video data using discrete cosine transform and coding techniques|
|[US5353303A](https://patents.google.com/patent/US5353303A/en)&nbsp;\*|1993-10-04|1994-10-04|The United States Of America As Represented By The Secretary Of The Air Force|Technique for increasing the data rate in a spread spectrum data link|
|[US5355450A](https://patents.google.com/patent/US5355450A/en)&nbsp;\*|1992-04-10|1994-10-11|Avid Technology, Inc.|Media composer with adjustable source material compression|
|[US5369502A](https://patents.google.com/patent/US5369502A/en)&nbsp;\*|1990-08-10|1994-11-29|Fujitsu Limited|Image data progressive reconstruction method and apparatus|
|[US5475431A](https://patents.google.com/patent/US5475431A/en)&nbsp;\*|1992-06-30|1995-12-12|Sony Corporation|Real-time encoding signal extraction and display apparatus|
|[US5491685A](https://patents.google.com/patent/US5491685A/en)&nbsp;\*|1994-05-19|1996-02-13|Digital Pictures, Inc.|System and method of digital compression and decompression using scaled quantization of variable-sized packets|
|[US5506687A](https://patents.google.com/patent/US5506687A/en)&nbsp;\*|1992-07-06|1996-04-09|Sony Corporation|Digital signal recording and/or reproducing apparatus that evenly distributes field data across tracks|
|[EP0817395A1](https://patents.google.com/patent/EP0817395A1/en)&nbsp;\*|1996-07-01|1998-01-07|Nec Corporation|Adaptive transform coding system and corresponding decoding system|
|[US5729633A](https://patents.google.com/patent/US5729633A/en)&nbsp;\*|1991-08-30|1998-03-17|Fuji Photo Film, Co. Ltd|Picture data compression coding device and method thereof|
|[US5781196A](https://patents.google.com/patent/US5781196A/en)&nbsp;\*|1990-10-19|1998-07-14|Eidos Plc Of The Boat House|Video compression by extracting pixel changes exceeding thresholds|
|[US5805302A](https://patents.google.com/patent/US5805302A/en)&nbsp;\*|1990-12-28|1998-09-08|Canon Kabushiki Kaisha|Variable length encoding of image data|
|[US5831559A](https://patents.google.com/patent/US5831559A/en)&nbsp;\*|1996-01-24|1998-11-03|Intel Corporation|Encoding/decoding video signals using multiple run-val mapping tables|
|[US20030025807A1](https://patents.google.com/patent/US20030025807A1/en)&nbsp;\*|1990-11-20|2003-02-06|Roberts Marc K.|Electronic still video camera with direct personal computer (PC) compatible digital format output|
|[US20040013786A1](https://patents.google.com/patent/US20040013786A1/en)&nbsp;\*|2001-05-03|2004-01-22|Mcardle Richard N.|Orange juice of low citrus oil content for reduction of heartburn episodes|
|[WO2012060821A1](https://patents.google.com/patent/WO2012060821A1/en)&nbsp;\*|2010-11-02|2012-05-10|Landmark Graphics Corporation|Systems and methods for generating updates of geological models|
|*Family To Family Citations*| | | | |
|[EP0474444B1](https://patents.google.com/patent/EP0474444B1/en)&nbsp;\*|1990-09-04|1999-12-01|Canon Kabushiki Kaisha|Image processing method and apparatus|
|[JPH04123587A](https://patents.google.com/patent/JPH04123587A/en)&nbsp;\*|1990-09-13|1992-04-23|Matsushita Electric Ind Co Ltd|Picture data compressor|

<small>\* Cited by examiner, † Cited by third party, ‡ Family to family citation</small>


### Similar Documents

|Publication|Publication Date|Title|
|:----------|:---------------|:----|
|[USRE44891E1](https://patents.google.com/patent/USRE44891E1/en)|2014-05-13|Systems and methods for reduced bit-depth processing in video-related data with frequency weighting matrices|
|[US20160119638A1](https://patents.google.com/patent/US20160119638A1/en)|2016-04-28|High dynamic range codecs|
|[US8705610B2](https://patents.google.com/patent/US8705610B2/en)|2014-04-22|Apparatus for encoding and decoding key data and key value data of coordinate interpolator and recording medium containing bitstream into which coordinate interpolator is encoded|
|[US5590139A](https://patents.google.com/patent/US5590139A/en)|1996-12-31|Method of recording a compressed motion picture signal in which effects of rounding errors resulting from inversely transforming transaction coefficients representing are mitigated|
|[EP0558016B1](https://patents.google.com/patent/EP0558016B1/en)|1999-09-15|Method and apparatus for encoding an image signal using a multi-stage quantizing number determiner|
|[CA2091815C](https://patents.google.com/patent/CA2091815C/en)|2000-05-23|Variable length code word decoder for use in digital communication systems|
|[US6310980B1](https://patents.google.com/patent/US6310980B1/en)|2001-10-30|Encoding apparatus and method and storage medium|
|[US5073821A](https://patents.google.com/patent/US5073821A/en)|1991-12-17|Orthogonal transform coding apparatus for reducing the amount of coded signals to be processed and transmitted|
|[JP3134424B2](https://patents.google.com/patent/JP3134424B2/en)|2001-02-13|Variable length encoding method and apparatus|
|[US5825830A](https://patents.google.com/patent/US5825830A/en)|1998-10-20|Method and apparatus for the compression of audio, video or other data|
|[US5355167A](https://patents.google.com/patent/US5355167A/en)|1994-10-11|Orthogonal transform coding apparatus|
|[EP0987899B1](https://patents.google.com/patent/EP0987899B1/en)|2005-02-09|Adaptive variable-length coding method for video data|
|[EP0619059B1](https://patents.google.com/patent/EP0619059B1/en)|1999-02-17|Method and apparatus for image data compression using combined luminance/chrominance coding|
|[CN101588502B](https://patents.google.com/patent/CN101588502B/en)|2013-04-24|DCT compression using Golomb-Rice coding|
|[US5481554A](https://patents.google.com/patent/US5481554A/en)|1996-01-02|Data transmission apparatus for transmitting code data|
|[CA2053844C](https://patents.google.com/patent/CA2053844C/en)|1997-01-21|Image data processing apparatus|
|[EP0447247B1](https://patents.google.com/patent/EP0447247B1/en)|1999-05-26|Image encoding method and apparatus|
|[US5930398A](https://patents.google.com/patent/US5930398A/en)|1999-07-27|Method and apparatus for determining a quantizing factor for multi-generation data compression/decompression processes|
|[EP0123456B1](https://patents.google.com/patent/EP0123456B1/en)|1990-12-12|A combined intraframe and interframe transform coding method|
|[US5440344A](https://patents.google.com/patent/US5440344A/en)|1995-08-08|Video encoder using adjacent pixel difference for quantizer control|
|[US6084909A](https://patents.google.com/patent/US6084909A/en)|2000-07-04|Method of encoding a stream of motion picture data|
|[US5724097A](https://patents.google.com/patent/US5724097A/en)|1998-03-03|Adaptive quantization of video based on edge detection|
|[EP1697897B1](https://patents.google.com/patent/EP1697897B1/en)|2014-05-21|Image processing|
|[US5654706A](https://patents.google.com/patent/US5654706A/en)|1997-08-05|System for variable length decoding digital transmission data which has been compressed by selecting a scanning pattern|
|[US20140112592A1](https://patents.google.com/patent/US20140112592A1/en)|2014-04-24|Image coding and decoding system using variable length coding and variable length decoding|


### Priority And Related Applications

### Priority Applications (2)

|Application|Priority date|Filing date|Title|
|:----------|:------------|:----------|:----|
|JP63-309871| |1988-12-09| |
|[JP63309871A](https://patents.google.com/patent/JPH0810935B2/en)|1988-12-09|1988-12-09|Image signal compression encoding device and decompression reproducing device|


### Legal Events


|Date|Code|Title|Description|
|:---|:---|:----|:----------|
|1990-02-01|AS|Assignment|<p>**Owner name**: FUJI PHOTO FILM CO., LTD., JAPAN</p><p>**Free format text**: ASSIGNMENT OF ASSIGNORS INTEREST.;ASSIGNORS:SAITO, OSAMU;ITO, KENJI;REEL/FRAME:005227/0822</p><p>**Effective date**: 19891219</p>|
|1990-11-01|STCF|Information on status: patent grant|**Free format text**: PATENTED CASE|
|1994-07-01|FPAY|Fee payment|**Year of fee payment**: 4|
|1998-06-10|FPAY|Fee payment|**Year of fee payment**: 8|
|2002-06-12|FPAY|Fee payment|**Year of fee payment**: 12|
|2007-02-15|AS|Assignment|<p>**Owner name**: FUJIFILM HOLDINGS CORPORATION, JAPAN</p><p>**Free format text**: CHANGE OF NAME;ASSIGNOR:FUJI PHOTO FILM CO., LTD.;REEL/FRAME:018898/0872</p><p>**Effective date**: 20061001</p><p>**Owner name**: FUJIFILM HOLDINGS CORPORATION,JAPAN</p><p>**Free format text**: CHANGE OF NAME;ASSIGNOR:FUJI PHOTO FILM CO., LTD.;REEL/FRAME:018898/0872</p><p>**Effective date**: 20061001</p>|
|2007-02-26|AS|Assignment|<p>**Owner name**: FUJIFILM CORPORATION, JAPAN</p><p>**Free format text**: ASSIGNMENT OF ASSIGNORS INTEREST;ASSIGNOR:FUJIFILM HOLDINGS CORPORATION;REEL/FRAME:018934/0001</p><p>**Effective date**: 20070130</p><p>**Owner name**: FUJIFILM CORPORATION,JAPAN</p><p>**Free format text**: ASSIGNMENT OF ASSIGNORS INTEREST;ASSIGNOR:FUJIFILM HOLDINGS CORPORATION;REEL/FRAME:018934/0001</p><p>**Effective date**: 20070130</p>|


### Concepts

<p title="The concepts are extracted by a computer and may be incomplete or incorrect."><small>machine-extracted</small></p>

[**<u>&#8681;&hairsp;</u>** &zwj; Download][csv-concepts]

|Name|Image|Sections|Count|Query match|
|:---|:----|:-------|:----|:----------|
|&#10152;&nbsp;compression| |title,claims,abstract,description|47|0.000|
|&#10152;&nbsp;transforming| |claims,description|72|0.000|

<small>Data provided by IFI CLAIMS Patent Services</small>



<!-- ====== URL REFERENCES ====== -->


[osamu-saito]: https://patents.google.com/?inventor=Osamu+Saito
[kenji-ito]: https://patents.google.com/?inventor=Kenji+Ito

<!-- EXTERNAL LINK REFERENCES -->

[external-uspto]: http://patft.uspto.gov/netacgi/nph-Parser?Sect1=PTO1&Sect2=HITOFF&p=1&u=/netahtml/PTO/srchnum.html&r=1&f=G&l=50&d=PALL&s1=4982282.PN.
[external-uspto-assignment]: https://assignment.uspto.gov/patent/index.html#/patent/search/resultFilter?searchInput=4982282
[external-espacenet]: http://worldwide.espacenet.com/publicationDetails/biblio?CC=US&NR=4982282A&KC=A&FT=D
[external-global-dossier]: http://globaldossier.uspto.gov/#/result/patent/US/4982282/1
[external-discuss]: https://patents.stackexchange.com/questions/tagged/US4982282


<!-- GOOGLE PAGE ORIGINAL REFERENCES -->

[google-patent-page]: https://patents.google.com/patent/US4982282A/en
[google-pdf-document]: https://patentimages.storage.googleapis.com/d0/04/ac/c37d8c992e9716/US4982282.pdf

<!-- [google-csv-concepts]: https://patents.google.com/860d2110-71ca-4d35-9092-93719f385b0b -->

[pdf-document]: ./img/patent/US4982282.pdf
[csv-concepts]: ./img/patent/US4982282A_concepts.csv


<!-- PAGE IMAGE URL REFERENCES -->

[thumb-2]: ./img/patent/US4982282-drawings-page-2_thumb.png  "FIG. 1"
[thumb-2-rot90]: ./img/patent/US4982282-drawings-page-2-rot90_thumb.png  "FIG. 1 (rotated 90deg)"
[thumb-3]: ./img/patent/US4982282-drawings-page-3_thumb.png  "FIG. 2, 3"
[thumb-4]: ./img/patent/US4982282-drawings-page-4_thumb.png  "FIG. 4A, 4B, 5"
[thumb-5]: ./img/patent/US4982282-drawings-page-5_thumb.png  "FIG. 6"
[thumb-5-rot90]: ./img/patent/US4982282-drawings-page-5-rot90_thumb.png  "FIG. 6 (rotated 90deg)"
[thumb-6]: ./img/patent/US4982282-drawings-page-6_thumb.png  "FIG. 7"
[thumb-6-rot90]: ./img/patent/US4982282-drawings-page-6-rot90_thumb.png  "FIG. 7 (rotated 90deg)"
[thumb-7]: ./img/patent/US4982282-drawings-page-7_thumb.png  "FIG. 8, 9"
[thumb-8]: ./img/patent/US4982282-drawings-page-8_thumb.png  "FIG. 10, 11"
[thumb-9]: ./img/patent/US4982282-drawings-page-9_thumb.png  "FIG. 12, 13"


[page-2]: ./img/patent/US4982282-drawings-page-2.png  "FIG. 1"
[page-2-rot90]: ./img/patent/US4982282-drawings-page-2-rot90.png  "FIG. 1 (rotated 90deg)"
[page-3]: ./img/patent/US4982282-drawings-page-3.png  "FIG. 2, 3"
[page-4]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4A, 4B, 5"
[page-5]: ./img/patent/US4982282-drawings-page-5.png  "FIG. 6"
[page-5-rot90]: ./img/patent/US4982282-drawings-page-5-rot90.png  "FIG. 6 (rotated 90deg)"
[page-6]: ./img/patent/US4982282-drawings-page-6.png  "FIG. 7"
[page-6-rot90]: ./img/patent/US4982282-drawings-page-6-rot90.png  "FIG. 7 (rotated 90deg)"
[page-7]: ./img/patent/US4982282-drawings-page-7.png  "FIG. 8, 9"
[page-8]: ./img/patent/US4982282-drawings-page-8.png  "FIG. 10, 11"
[page-9]: ./img/patent/US4982282-drawings-page-9.png  "FIG. 12, 13"


<!-- FIGURE IMAGE URL REFERENCES -->


[fig-1-orig]: ./img/patent/US4982282-drawings-page-2.png  "FIG. 1 is a schematic block diagram illustrating an embodiment of an image signal compression encoding apparatus in accordance with the present invention;"
[fig-1]: ./img/patent/US4982282-drawings-page-2-rot90.png  "FIG. 1 (rotated 90deg) is a schematic block diagram illustrating an embodiment of an image signal compression encoding apparatus in accordance with the present invention;"
[fig-2]: ./img/patent/US4982282-drawings-page-3.png  "FIG. 2 is a block diagram illustrating a non-zero sensor section 18 of FIG. 1;"
[fig-3]: ./img/patent/US4982282-drawings-page-3.png  "FIG. 3 is a schematic block diagram illustrating a zero-run counter 20 of FIG. 1;"
[fig-4a]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4A is a block diagram illustrating an amplitude sensor section 24 of FIG. 1;"
[fig-4b]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4B is a schematic diagram illustrating an overflow sensor circuit 50 of FIG. 4A;"
[fig-5]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 5 is a diagram schematically illustrating an additional-bit computing section 26 of FIG. 1;"
[fig-6-orig]: ./img/patent/US4982282-drawings-page-5.png  "FIG. 6 is a schematic diagram illustrating a fixed-length item generating buffer 30;"
[fig-6]: ./img/patent/US4982282-drawings-page-5-rot90.png  "FIG. 6 (rotated 90deg) is a schematic diagram illustrating a fixed-length item generating buffer 30;"
[fig-7-orig]: ./img/patent/US4982282-drawings-page-6.png  "FIG. 7 is a block diagram schematically illustrating an image signal expansion reproducing apparatus for decoding and for reproducing image data compressed and encoded by the compression encoding apparatus of FIG. 1;"
[fig-7]: ./img/patent/US4982282-drawings-page-6-rot90.png  "FIG. 7 (rotated 90deg) is a block diagram schematically illustrating an image signal expansion reproducing apparatus for decoding and for reproducing image data compressed and encoded by the compression encoding apparatus of FIG. 1;"
[fig-8]: ./img/patent/US4982282-drawings-page-7.png  "FIG. 8 is a diagram illustrating data obtained through a two-dimensional orthogonal transformation;"
[fig-9]: ./img/patent/US4982282-drawings-page-7.png  "FIG. 9 is a schematic diagram illustrating encoding operations associated with the run length and a non-zero amplitude;"
[fig-10]: ./img/patent/US4982282-drawings-page-8.png  "FIG. 10 is a diagram illustrating an example of weight table data;"
[fig-11]: ./img/patent/US4982282-drawings-page-8.png  "FIG. 11 is a schematic diagram illustrating an example of a normalized transformation coefficient;"
[fig-12]: ./img/patent/US4982282-drawings-page-9.png  "FIG. 12 is a diagram illustrating relationships between amplitude values of the translation coefficient, amplitude ranges thereof, and additional bits; and"
[fig-13]: ./img/patent/US4982282-drawings-page-9.png  "FIG. 13 is a schematic diagram illustrating relationships between amplitude values of the transformation coefficient and amplitude ranges thereof."


<!--
[fig-1-orig]: ./img/patent/US4982282-drawings-page-2.png  "FIG. 1"
[fig-1]: ./img/patent/US4982282-drawings-page-2-rot90.png  "FIG. 1 (rotated 90deg)"
[fig-2]: ./img/patent/US4982282-drawings-page-3.png  "FIG. 2, 3"
[fig-3]: ./img/patent/US4982282-drawings-page-3.png  "FIG. 2, 3"
[fig-4a]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4A, 4B, 5"
[fig-4b]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4A, 4B, 5"
[fig-5]: ./img/patent/US4982282-drawings-page-4.png  "FIG. 4A, 4B, 5"
[fig-6-orig]: ./img/patent/US4982282-drawings-page-5.png  "FIG. 6"
[fig-6]: ./img/patent/US4982282-drawings-page-5-rot90.png  "FIG. 6 (rotated 90deg)"
[fig-7-orig]: ./img/patent/US4982282-drawings-page-6.png  "FIG. 7"
[fig-7]: ./img/patent/US4982282-drawings-page-6-rot90.png  "FIG. 7 (rotated 90deg)"
[fig-8]: ./img/patent/US4982282-drawings-page-7.png  "FIG. 8, 9"
[fig-9]: ./img/patent/US4982282-drawings-page-7.png  "FIG. 8, 9"
[fig-10]: ./img/patent/US4982282-drawings-page-8.png  "FIG. 10, 11"
[fig-11]: ./img/patent/US4982282-drawings-page-8.png  "FIG. 10, 11"
[fig-12]: ./img/patent/US4982282-drawings-page-9.png  "FIG. 12, 13"
[fig-13]: ./img/patent/US4982282-drawings-page-9.png  "FIG. 12, 13"
-->


<!-- SECTION ID REFERENCES -->

[table-of-contents]: #Table%20of%20Contents

[title]: #US4982282A
[heading]: #Image%20signal%20compression%20encoding%20apparatus%20and%20image%20signal%20expansion%20reproducing%20apparatus

[us4982282a]: #US4982282A
[image-signal-compression-encoding-apparatus-and-image-signal-expansion-reproducing-apparatus]: #Image%20signal%20compression%20encoding%20apparatus%20and%20image%20signal%20expansion%20reproducing%20apparatus


[abstract]: #Abstract
[images]: #Images%20%288%29
[classifications]: #Classifications

[description]: #Description
[background-of-the-invention]: #BACKGROUND%20OF%20THE%20INVENTION
[summary-of-the-invention]: #SUMMARY%20OF%20THE%20INVENTION
[brief-description-of-the-drawings]: #BRIEF%20DESCRIPTION%20OF%20THE%20DRAWINGS
[description-of-the-preferred-embodiments]: #DESCRIPTION%20OF%20THE%20PREFERRED%20EMBODIMENTS

[claims]: #Claims%20%2813%29


[patent-citations]: #Patent%20Citations%20%284%29
[cited-by]: #Cited%20By%20%2839%29
[similar-documents]: #Similar%20Documents
[priority-and-related-applications]: #Priority%20And%20Related%20Applications
[priority-applications]: #Priority%20Applications%20%282%29
[legal-events]: #Legal%20Events
[concepts]: #Concepts


