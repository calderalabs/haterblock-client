open Belt;

open Jest;

open CommentData.Sentiment;

open CommentData.Comment;

describe("CommentData.Sentiment.filterBySentiment", () => {
  open Expect;
  let positiveComments = [
    {id: 7, body: "This is really good", score: 5, status: Published},
  ];
  let neutralComments = [
    {
      id: 2,
      body: "This is not so great but ok",
      score: (-3),
      status: Published,
    },
  ];
  let negativeComments = [
    {id: 1, body: "I don't like this", score: (-6), status: Published},
    {id: 3, body: "Awful content mate", score: (-5), status: Published},
  ];
  let hatefulComments = [
    {id: 6, body: "This is total garbage", score: (-9), status: Published},
    {id: 4, body: "This is fucking shit", score: (-8), status: Published},
    {id: 5, body: "You should kill yourself", score: (-7), status: Published},
  ];
  let comments =
    List.concatMany([|
      positiveComments,
      neutralComments,
      negativeComments,
      hatefulComments,
    |]);
  test("it filters positive comments", () =>
    expect(filterBySentiment(comments, Positive))
    |> toEqual(positiveComments)
  );
  test("it filters neutral comments", () =>
    expect(filterBySentiment(comments, Neutral)) |> toEqual(neutralComments)
  );
  test("it filters negative comments", () =>
    expect(filterBySentiment(comments, Negative))
    |> toEqual(negativeComments)
  );
  test("it filters hateful comments", () =>
    expect(filterBySentiment(comments, Hateful)) |> toEqual(hatefulComments)
  );
});
