open Belt;

[%bs.raw {|require('./CommentListFilters.css')|}];

let component = ReasonReact.statelessComponent("CommentListFilters");

type filters = {
  status: list(CommentData.Status.t),
  sentiment: list(CommentData.Sentiment.t),
};

let make =
    (
      ~filters: filters,
      ~onFiltersChange: filters => unit,
      ~counts: Js.Dict.t(int),
      _children,
    ) => {
  let isStatusChecked = (status: CommentData.Status.t) =>
    filters.status |> List.has(_, status, (==));
  let onToggleStatus = (status: CommentData.Status.t) => {
    let newStatus =
      if (isStatusChecked(status)) {
        filters.status |> List.keep(_, s => s != status);
      } else {
        filters.status |> List.add(_, status);
      };
    onFiltersChange({...filters, status: newStatus});
  };
  let isSentimentChecked = (sentiment: CommentData.Sentiment.t) =>
    filters.sentiment |> List.has(_, sentiment, (==));
  let onToggleSentiment = (sentiment: CommentData.Sentiment.t) => {
    let newSentiment =
      if (isSentimentChecked(sentiment)) {
        filters.sentiment |> List.keep(_, s => s != sentiment);
      } else {
        filters.sentiment |> List.add(_, sentiment);
      };
    onFiltersChange({...filters, sentiment: newSentiment});
  };
  let publishedCommentsLabel =
    switch (Js.Dict.get(counts, "published_comments")) {
    | Some(publishedCommentsCount) =>
      CommentData.Status.toString(CommentData.Status.Published)
      ++ {j| ($publishedCommentsCount)|j}
    | None => CommentData.Status.toString(CommentData.Status.Published)
    };
  let rejectedCommentsLabel =
    switch (Js.Dict.get(counts, "rejected_comments")) {
    | Some(rejectedCommentsCount) =>
      CommentData.Status.toString(CommentData.Status.Rejected)
      ++ {j| ($rejectedCommentsCount)|j}
    | None => CommentData.Status.toString(CommentData.Status.Rejected)
    };
  let hatefulCommentsLabel =
    switch (Js.Dict.get(counts, "hateful_comments")) {
    | Some(hatefulCommentsCount) =>
      CommentData.Sentiment.toString(CommentData.Sentiment.Hateful)
      ++ {j| ($hatefulCommentsCount)|j}
    | None => CommentData.Sentiment.toString(CommentData.Sentiment.Hateful)
    };
  let negativeCommentsLabel =
    switch (Js.Dict.get(counts, "negative_comments")) {
    | Some(negativeCommentsCount) =>
      CommentData.Sentiment.toString(CommentData.Sentiment.Negative)
      ++ {j| ($negativeCommentsCount)|j}
    | None => CommentData.Sentiment.toString(CommentData.Sentiment.Negative)
    };
  let neutralCommentsLabel =
    switch (Js.Dict.get(counts, "neutral_comments")) {
    | Some(neutralCommentsCount) =>
      CommentData.Sentiment.toString(CommentData.Sentiment.Neutral)
      ++ {j| ($neutralCommentsCount)|j}
    | None => CommentData.Sentiment.toString(CommentData.Sentiment.Neutral)
    };
  let positiveCommentsLabel =
    switch (Js.Dict.get(counts, "positive_comments")) {
    | Some(positiveCommentsCount) =>
      CommentData.Sentiment.toString(CommentData.Sentiment.Positive)
      ++ {j| ($positiveCommentsCount)|j}
    | None => CommentData.Sentiment.toString(CommentData.Sentiment.Positive)
    };
  {
    ...component,
    render: _self =>
      <div className="CommentListFilters">
        <div className="CommentListFilters__filter">
          <Checkbox
            label=publishedCommentsLabel
            checked=(isStatusChecked(CommentData.Status.Published))
            onChange=(() => onToggleStatus(CommentData.Status.Published))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=rejectedCommentsLabel
            checked=(isStatusChecked(CommentData.Status.Rejected))
            onChange=(() => onToggleStatus(CommentData.Status.Rejected))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=hatefulCommentsLabel
            checked=(isSentimentChecked(CommentData.Sentiment.Hateful))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Hateful))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=negativeCommentsLabel
            checked=(isSentimentChecked(CommentData.Sentiment.Negative))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Negative))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=neutralCommentsLabel
            checked=(isSentimentChecked(CommentData.Sentiment.Neutral))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Neutral))
            className="Checkbox--small"
          />
        </div>
        <div className="CommentListFilters__filter">
          <Checkbox
            label=positiveCommentsLabel
            checked=(isSentimentChecked(CommentData.Sentiment.Positive))
            onChange=(() => onToggleSentiment(CommentData.Sentiment.Positive))
            className="Checkbox--small"
          />
        </div>
      </div>,
  };
};
